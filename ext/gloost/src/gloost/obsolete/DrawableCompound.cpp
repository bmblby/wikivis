
/*
                       ___                            __
                      /\_ \                          /\ \__
                   __ \//\ \     ___     ___     ____\ \  _\
                 /'_ `\ \ \ \   / __`\  / __`\  /  __\\ \ \/
                /\ \ \ \ \_\ \_/\ \ \ \/\ \ \ \/\__   \\ \ \_
                \ \____ \/\____\ \____/\ \____/\/\____/ \ \__\
                 \/___/\ \/____/\/___/  \/___/  \/___/   \/__/
                   /\____/
                   \_/__/

                   OpenGL framework for fast demo programming

                             http://www.gloost.org

    This file is part of the gloost framework. You can use it in parts or as
       whole under the terms of the GPL (http://www.gnu.org/licenses/#GPL).
*/



// gloost system includes
#include <gloost/obsolete/DrawableCompound.h>
#include <gloost/Mesh.h>
#include <gloost/obsolete/Vbo.h>



// cpp includes
#include <string>
#include <iostream>



namespace gloost
{
namespace obsolete
{

/**
  \class   DrawableCompound

  \brief   Compound containing a Mesh and a Vbo to use them together

  \author  Felix Weiszig
  \date    June 2011
  \remarks
*/

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

DrawableCompound::DrawableCompound():
    _mesh(gloost::Mesh::create()),
    _vbo(new gloost::obsolete::Vbo(_mesh)),
    _meshDirty(true)
{
	_vbo->takeReference();
}

////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class constructor
  \remarks ...
*/

DrawableCompound::DrawableCompound(std::shared_ptr<Mesh> mesh):
    _mesh(mesh),
    _vbo(new gloost::obsolete::Vbo(_mesh)),
    _meshDirty(true)
{
	_vbo->takeReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   Class destructor
  \remarks ...
*/

DrawableCompound::~DrawableCompound()
{
	_vbo->dropReference();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   sets the Mesh dirty to indicate a change within its content and trigger an Vbo update
  \param   ...
  \remarks ...
*/

void
DrawableCompound::setMeshDirty(bool value)
{
	_meshDirty = value;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   updates the vbo, necessary if Mesh was changed
  \param   ...
  \remarks ...
*/

void
DrawableCompound::updateVbo()
{
	if (_meshDirty)
	{
	  unsigned drawMode = _vbo->getDrawMode();
    _vbo->dropReference();
    _vbo = new gloost::obsolete::Vbo(_mesh);
    _vbo->takeReference();
    _vbo->setDrawMode(drawMode);
    _meshDirty = false;
	}
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   clears the Mesh and removes resources from GPU
  \param   ...
  \remarks ...
*/

void
DrawableCompound::clear()
{
	setMesh(gloost::Mesh::create());
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   replaces the current Mesh with another one
  \param   ...
  \remarks ...
*/

void
DrawableCompound::setMesh(std::shared_ptr<Mesh> mesh)
{
  _mesh = mesh;
	setMeshDirty(true);
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Mesh
  \param   ...
  \remarks ...
*/

std::shared_ptr<Mesh>
DrawableCompound::getMesh()
{
	return _mesh;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   returns the Vbo
  \param   ...
  \remarks ...
*/

Vbo*
DrawableCompound::getVbo()
{
	return _vbo;
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   draws the compound
  \param   ...
  \remarks ...
*/

void
DrawableCompound::draw()
{
  _vbo->bind();
	_vbo->draw();
	_vbo->disable();
}


////////////////////////////////////////////////////////////////////////////////


/**
  \brief   draws the compound
  \param   ...
  \remarks ...
*/

void
DrawableCompound::updateAndDraw()
{
	updateVbo();
  draw();
}


////////////////////////////////////////////////////////////////////////////////





} // namespace obsolete
} // namespace gloost



