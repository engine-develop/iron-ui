//------------------------------------------------------------------------------
//

// TinyXML2
#include <tinyxml2.h>

// Engine
#include "graph.hpp"
#include "variable.hpp"
#include "node.hpp"

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

    QList< QGraphicsItem* > components = graph->components();

    foreach( QGraphicsItem* component, components )
    {
        switch ( component->type() )
        {
            case Variable::Type:
            {
                Variable* variable = static_cast< Variable* >( component );
                if ( variable->node() ) break;

                tinyxml2::XMLElement* variableElem = doc->NewElement( "variable" );
                variableElem->SetAttribute( "typeId", variable->typeId() );
                variableElem->SetAttribute( "name", variable->name().toStdString().c_str() );
                variableElem->SetAttribute( "scenePosX", variable->scenePos().x() );
                variableElem->SetAttribute( "scenePosY", variable->scenePos().y() );

                componentsElem->InsertEndChild( variableElem );

                break;
            }

            case Node::Type:
            {
                Node* node = static_cast< Node* >( component );

                tinyxml2::XMLElement* nodeElem = doc->NewElement( "node" );
                nodeElem->SetAttribute( "typeId", node->typeId() );
                nodeElem->SetAttribute( "name", node->name().toStdString().c_str() );
                nodeElem->SetAttribute( "scenePosX", node->scenePos().x() );
                nodeElem->SetAttribute( "scenePosY", node->scenePos().y() );

                componentsElem->InsertEndChild( nodeElem );

                break;
            }

            default:
            {
                break;
            }
        }
    }

    graphElem->InsertEndChild( componentsElem );

    // Write connections
    //
    tinyxml2::XMLElement* connectionsElem = doc->NewElement( "connections" );
    graphElem->InsertEndChild( connectionsElem );
}

//------------------------------------------------------------------------------
//

bool write( Sketch* sketch,
            const std::string& path )
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

        uint32_t typeId = 0;
        child->QueryUnsignedAttribute( "typeId", &typeId );

        const char* name = child->Attribute( "name" );

        int scenePosX = 0;
        child->QueryIntAttribute( "scenePosX", &scenePosX );

        int scenePosY = 0;
        child->QueryIntAttribute( "scenePosY", &scenePosY );

        if ( elemName == "variable" )
        {
            Variable* variable = graph->createVariable( typeId );
            variable->setName( name );
            variable->setPos( scenePosX, scenePosY );
        }

        else if ( elemName == "node" )
        {
            Node* node = graph->createNode( typeId );
            node->setName( name );
            node->setPos( scenePosX, scenePosY );
        }
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
