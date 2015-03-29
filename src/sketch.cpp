//------------------------------------------------------------------------------
//

// TinyXML2
#include <tinyxml2.h>

// Engine
#include "graph.hpp"
#include "variable.hpp"
#include "node.hpp"
#include "connection.hpp"
#include "port.hpp"

#include "sketch.hpp"

//------------------------------------------------------------------------------
//

Sketch::Sketch( QWidget* parent )
    : QTabWidget( parent )
    , m_setupGraph( 0x0 )
    , m_loopGraph( 0x0 )
{
    m_setupGraph = new Graph( Graph::Type_Setup, this );
    m_loopGraph  = new Graph( Graph::Type_Loop,  this );

    addTab( m_setupGraph->view(), "Setup" );
    addTab( m_loopGraph->view(),  "Loop" );
}

//------------------------------------------------------------------------------
//

Sketch::~Sketch()
{
}

//------------------------------------------------------------------------------
//

void Sketch::clear()
{
    // Clear graphs
    //
    m_setupGraph->clear();
    m_loopGraph->clear();

    // Reset views
    //
    m_setupGraph->view()->setMatrix( QMatrix() );
    m_loopGraph->view()->setMatrix( QMatrix() );
}

//------------------------------------------------------------------------------
//

Graph* Sketch::setupGraph()
{
    return m_setupGraph;
}

//------------------------------------------------------------------------------
//

Graph* Sketch::loopGraph()
{
    return m_loopGraph;
}

//------------------------------------------------------------------------------
//

void writeVariable( tinyxml2::XMLElement* variableElem,
                    Variable* v )
{
    variableElem->SetAttribute( "id", v->id() );
    variableElem->SetAttribute( "typeId", v->typeId() );
    variableElem->SetAttribute( "name", v->name().toStdString().c_str() );
    variableElem->SetAttribute( "scenePosX", v->scenePos().x() );
    variableElem->SetAttribute( "scenePosY", v->scenePos().y() );
    variableElem->SetAttribute( "portExpanded", v->port()->expanded() );
    variableElem->SetAttribute( "portValue", v->port()->value().toStdString().c_str() );
}

//------------------------------------------------------------------------------
//

void writeGraph( tinyxml2::XMLDocument* doc,
                 tinyxml2::XMLNode* graphElem,
                 Graph* graph )
{
    // Write view
    //
    tinyxml2::XMLElement* viewElem = doc->NewElement( "view" );
    QMatrix viewMat = graph->view()->matrix();
    viewElem->SetAttribute( "matrixM11", viewMat.m11() );
    viewElem->SetAttribute( "matrixM12", viewMat.m12() );
    viewElem->SetAttribute( "matrixM21", viewMat.m21() );
    viewElem->SetAttribute( "matrixM22", viewMat.m22() );
    viewElem->SetAttribute( "matrixDX", viewMat.dx() );
    viewElem->SetAttribute( "matrixDY", viewMat.dy() );
    graphElem->InsertEndChild( viewElem );

    // Write components
    //
    tinyxml2::XMLElement* componentsElem = doc->NewElement( "components" );

    QList< QGraphicsItem* > variables = graph->componentsByType( Variable::Type );

    foreach( QGraphicsItem* variable, variables )
    {
        Variable* v = static_cast< Variable* >( variable );

        if ( v->node() )
        {
            continue;
        }

        tinyxml2::XMLElement* variableElem = doc->NewElement( "variable" );
        writeVariable( variableElem, v );

        componentsElem->InsertEndChild( variableElem );
    }

    QList< QGraphicsItem* > nodes = graph->componentsByType( Node::Type );

    foreach( QGraphicsItem* node, nodes )
    {
        Node* n = static_cast< Node* >( node );

        tinyxml2::XMLElement* nodeElem = doc->NewElement( "node" );
        nodeElem->SetAttribute( "id", n->id() );
        nodeElem->SetAttribute( "typeId", n->typeId() );
        nodeElem->SetAttribute( "name", n->name().toStdString().c_str() );
        nodeElem->SetAttribute( "scenePosX", n->scenePos().x() );
        nodeElem->SetAttribute( "scenePosY", n->scenePos().y() );

        tinyxml2::XMLElement* attributesElem = doc->NewElement( "attributes" );

        foreach( Variable* attribute, n->attributes() )
        {
            tinyxml2::XMLElement* variableElem = doc->NewElement( "variable" );
            writeVariable( variableElem, attribute );
            attributesElem->InsertEndChild( variableElem );
        }

        nodeElem->InsertEndChild( attributesElem );

        componentsElem->InsertEndChild( nodeElem );
    }

    graphElem->InsertEndChild( componentsElem );

    // Write connections
    //
    tinyxml2::XMLElement* connectionsElem = doc->NewElement( "connections" );

    QList< QGraphicsItem* > connections = graph->componentsByType( Connection::Type );

    foreach( QGraphicsItem* connection, connections )
    {
        Connection* c = static_cast< Connection* >( connection );

        tinyxml2::XMLElement* connectionElem = doc->NewElement( "connection" );

        uint32_t port1Id1 = 0;
        uint32_t port1Id2 = 0;
        uint32_t port2Id1 = 0;
        uint32_t port2Id2 = 0;

        if ( c->port1()->variable()->node() )
        {
            port1Id1 = c->port1()->variable()->node()->id();
            port1Id2 = c->port1()->variable()->attributeId();
        }

        else
        {
            port1Id1 = c->port1()->variable()->id();
        }

        if ( c->port2()->variable()->node() )
        {
            port2Id1 = c->port2()->variable()->node()->id();
            port2Id2 = c->port2()->variable()->attributeId();
        }

        else
        {
            port2Id1 = c->port2()->variable()->id();
        }

        connectionElem->SetAttribute( "port1Id1", port1Id1 );
        connectionElem->SetAttribute( "port1Id2", port1Id2 );
        connectionElem->SetAttribute( "port2Id1", port2Id1 );
        connectionElem->SetAttribute( "port2Id2", port2Id2 );
        connectionsElem->InsertEndChild( connectionElem );
    }

    graphElem->InsertEndChild( connectionsElem );
}

//------------------------------------------------------------------------------
//

bool write( Sketch* sketch,
            std::string path )
{
    if ( !sketch || path.empty() ) return false;

    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

    tinyxml2::XMLNode* rootElem = doc->NewElement( "sketch" );
    doc->InsertFirstChild( rootElem );

    // Write setup graph
    //
    tinyxml2::XMLNode* setupGraphElem = doc->NewElement( "setupGraph" );
    writeGraph( doc, setupGraphElem, sketch->setupGraph() );
    rootElem->InsertEndChild( setupGraphElem );

    // Write loop graph
    //
    tinyxml2::XMLNode* loopGraphElem = doc->NewElement( "loopGraph" );
    writeGraph( doc, loopGraphElem, sketch->loopGraph() );
    rootElem->InsertEndChild( loopGraphElem );

    tinyxml2::XMLError status = doc->SaveFile( path.c_str() );

    delete doc;

    return ( status == tinyxml2::XML_SUCCESS );
}

//------------------------------------------------------------------------------
//

void readVariable( tinyxml2::XMLElement* variableElem,
                   Graph* graph,
                   Variable* variable )
{
    uint32_t id = 0;
    variableElem->QueryUnsignedAttribute( "id", &id );
    uint32_t typeId = 0;
    variableElem->QueryUnsignedAttribute( "typeId", &typeId );
    const char* name = variableElem->Attribute( "name" );
    int scenePosX = 0;
    variableElem->QueryIntAttribute( "scenePosX", &scenePosX );
    int scenePosY = 0;
    variableElem->QueryIntAttribute( "scenePosY", &scenePosY );
    bool portExpanded = false;
    variableElem->QueryBoolAttribute( "portExpanded", &portExpanded );
    const char* portValue = variableElem->Attribute( "portValue" );

    variable->setName( name );

    // Do not set position if part of Node
    //
    if ( !variable->node() )
    {
        variable->setPos( scenePosX, scenePosY );
    }

    variable->port()->setExpanded( portExpanded );
    variable->port()->setValue( portValue );

    variable->update();

    graph->switchComponentId( variable->id(), id );
}

//------------------------------------------------------------------------------
//

void readVariable( tinyxml2::XMLElement* variableElem,
                   Graph* graph )
{
    uint32_t typeId = 0;
    variableElem->QueryUnsignedAttribute( "typeId", &typeId );

    Variable* variable = graph->createVariable( typeId );

    readVariable( variableElem, graph, variable );
}

//------------------------------------------------------------------------------
//

void readNode( tinyxml2::XMLElement* nodeElem,
               Graph* graph )
{
    uint32_t id = 0;
    nodeElem->QueryUnsignedAttribute( "id", &id );
    uint32_t typeId = 0;
    nodeElem->QueryUnsignedAttribute( "typeId", &typeId );
    const char* name = nodeElem->Attribute( "name" );
    int scenePosX = 0;
    nodeElem->QueryIntAttribute( "scenePosX", &scenePosX );
    int scenePosY = 0;
    nodeElem->QueryIntAttribute( "scenePosY", &scenePosY );

    Node* node = graph->createNode( typeId );
    node->setName( name );
    node->setPos( scenePosX, scenePosY );

    // Read attributes
    //
    QList< Variable* > attributes = node->attributes();
    tinyxml2::XMLElement* attributesElem = nodeElem->FirstChildElement();

    // Count number of attributes
    //
    uint8_t numAttributes = 0;

    for ( tinyxml2::XMLElement* child = attributesElem->FirstChildElement();
          child != 0x0; child = child->NextSiblingElement() )
    {
        ++numAttributes;
    }

    // Check number of attributes match
    //
    if ( numAttributes == attributes.size() )
    {
        numAttributes = 0;

        for ( tinyxml2::XMLElement* child = attributesElem->FirstChildElement();
              child != 0x0; child = child->NextSiblingElement() )
        {
            readVariable( child, graph, attributes[ numAttributes++ ] );
        }
    }

    node->update();

    graph->switchComponentId( node->id(), id );
}

//------------------------------------------------------------------------------
//

void readGraph( tinyxml2::XMLElement* graphElem,
                Graph* graph )
{
    // Read view
    //
    tinyxml2::XMLElement* viewElem = graphElem->FirstChildElement();
    double matrixM11 = 0;
    viewElem->QueryDoubleAttribute( "matrixM11", &matrixM11 );
    double matrixM12 = 0;
    viewElem->QueryDoubleAttribute( "matrixM12", &matrixM12 );
    double matrixM21 = 0;
    viewElem->QueryDoubleAttribute( "matrixM21", &matrixM21 );
    double matrixM22 = 0;
    viewElem->QueryDoubleAttribute( "matrixM22", &matrixM22 );
    double matrixDX = 0;
    viewElem->QueryDoubleAttribute( "matrixDX", &matrixDX );
    double matrixDY = 0;
    viewElem->QueryDoubleAttribute( "matrixDY", &matrixDY );

    QMatrix viewMat( matrixM11, matrixM12, matrixM21, matrixM22, matrixDX, matrixDY );
    graph->view()->setMatrix( viewMat );

    // Read components
    //
    tinyxml2::XMLElement* componentsElem = viewElem->NextSiblingElement();

    for ( tinyxml2::XMLElement* child = componentsElem->FirstChildElement();
          child != 0x0; child = child->NextSiblingElement() )
    {
        std::string elemName = child->Name();

        if ( elemName == "variable" )
        {
            readVariable( child, graph );
        }

        else if ( elemName == "node" )
        {
            readNode( child, graph );
        }
    }

    // Read connections
    //
    tinyxml2::XMLElement* connectionsElem = componentsElem->NextSiblingElement();

    for ( tinyxml2::XMLElement* child = connectionsElem->FirstChildElement();
          child != 0x0; child = child->NextSiblingElement() )
    {
        uint32_t port1Id1 = 0;
        uint32_t port1Id2 = 0;
        uint32_t port2Id1 = 0;
        uint32_t port2Id2 = 0;

        child->QueryUnsignedAttribute( "port1Id1", &port1Id1 );
        child->QueryUnsignedAttribute( "port1Id2", &port1Id2 );
        child->QueryUnsignedAttribute( "port2Id1", &port2Id1 );
        child->QueryUnsignedAttribute( "port2Id2", &port2Id2 );

        QGraphicsItem* component1 = graph->component( port1Id1 );
        QGraphicsItem* component2 = graph->component( port2Id1 );

        if ( !component1 || !component2 )
        {
            continue;
        }

        Port* port1 = 0x0;

        if ( component1->type() == Variable::Type )
        {
            Variable* v = static_cast< Variable* >( component1 );
            port1 = v->port();
        }

        else if ( component1->type() == Node::Type )
        {
            Node* n = static_cast< Node* >( component1 );
            Variable* v = n->attribute( port1Id2 );
            if ( v ) port1 = v->port();
        }

        Port* port2 = 0x0;

        if ( component2->type() == Variable::Type )
        {
            Variable* v = static_cast< Variable* >( component2 );
            port2 = v->port();
        }

        else if ( component2->type() == Node::Type )
        {
            Node* n = static_cast< Node* >( component2 );
            Variable* v = n->attribute( port2Id2 );
            if ( v ) port2 = v->port();
        }

        graph->createConnection( port1, port2 );
    }
}

//------------------------------------------------------------------------------
//

bool read( const std::string& path,
           Sketch* sketch )
{
    if ( path.empty() || !sketch ) return false;

    sketch->clear();

    tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
    doc->LoadFile( path.c_str() );

    tinyxml2::XMLElement* rootElem = doc->FirstChildElement();

    // Read setup graph
    //
    tinyxml2::XMLElement* setupGraphElem = rootElem->FirstChildElement();
    readGraph( setupGraphElem, sketch->setupGraph() );

    // Read loop graph
    //
    tinyxml2::XMLElement* loopGraphElem = setupGraphElem->NextSiblingElement();
    readGraph( loopGraphElem, sketch->loopGraph() );

    delete doc;

    return true;
}
