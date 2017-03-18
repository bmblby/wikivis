
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



#include <gloost/ShadowMapPointLight.h>
#include <gloost/TextureManager.h>
#include <gloost/gloostMath.h>


namespace gloost
{

///////////////////////////////////////////////////////////////////////////////


  /// class constructor

ShadowMapPointLight::ShadowMapPointLight(unsigned int smap_width,
                                         unsigned int smap_height):
    PointLight(),
    _viewport(0,0,smap_width,smap_height),
    _camera(0),
    _fbo(0),
    _shadowMapTexture(0),
    _shadowMapTexture_handle(0),
    _shadowMapTexture_id(0),
    _polygonOffsetFactor(3.0),
    _polygonOffsetUnits(3.0)
{


  _camera = new PerspectiveCamera;
  _camera->setAspectRatio((GLfloat)(smap_width)/(GLfloat)(smap_height));


  _fbo = new gl::Fbo;


  // generate DepthTexture
  _shadowMapTexture = new gl::Texture(smap_width,
                                   smap_height,
                                   GL_TEXTURE_2D,
                                   GL_DEPTH_COMPONENT32,
                                   GL_DEPTH_COMPONENT,
                                   GL_FLOAT);


  _shadowMapTexture->setTexParameter(GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
  _shadowMapTexture->setTexParameter(GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
  _shadowMapTexture->enableTextureMatrix();

  _shadowMapTexture_id = TextureManager::getInstance()->addTexture(_shadowMapTexture);


  _shadowMapTexture_handle = _shadowMapTexture->getTextureHandle();

  _fbo->attachTexture(_shadowMapTexture->getTarget(),
                       _shadowMapTexture_handle,
                       GL_DEPTH_ATTACHMENT);

  // unset the fbo
  gl::Fbo::unbind();

}


///////////////////////////////////////////////////////////////////////////////


  ///

/* virtual */
ShadowMapPointLight::~ShadowMapPointLight()
{
  delete _fbo;
  delete _camera;
}


///////////////////////////////////////////////////////////////////////////////


  ///

void
ShadowMapPointLight::beginUniformShadowMapCreation(const Point3& sceneMin,
                                                   const Point3& sceneMax,
                                                   const Camera* cam)
{

  beginUniformShadowMapCreation(sceneMin, sceneMax);

  Matrix t;
  t.setIdentity();
  t.setTranslate(0.5,0.5,0.5);
  Matrix s;
  s.setIdentity();
  s.setScale(0.5,0.5,0.5);

  Matrix newShadowmapMatrix(cam->getViewMatrix());
  newShadowmapMatrix.invert();
  newShadowmapMatrix.mult(_camera->getViewMatrix());
  newShadowmapMatrix.mult(_camera->getProjectionMatrix());
  newShadowmapMatrix.mult(s);
  newShadowmapMatrix.mult(t);

  _shadowMapTexture->setTextureMatrix(newShadowmapMatrix);
}

///////////////////////////////////////////////////////////////////////////////


  ///

void
ShadowMapPointLight::beginUniformShadowMapCreation(const Point3& sceneMin,
                                                   const Point3& sceneMax)
{
  // enter fbo
  _fbo->bind();

  _viewport.enter();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDrawBuffer(GL_NONE);
  glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
  glClear(GL_DEPTH_BUFFER_BIT);


  // calc matrices...use matrixcamera in future
  // looking into the middle of the boundingbox
  const Point3 lightpos(_position[0],_position[1],_position[2]);

  const Point3 origin(0.0,0.0,0.0);

  /*
  const Point3 scene_middle(((Vector3(sceneMin) - origin) +
                             (Vector3(sceneMax) - origin)).scalar(0.5));
  */

  const Point3 scene_middle( sceneMin + 0.5*(sceneMax - sceneMin)  );


  const float scene_radius(Vector3(sceneMax - sceneMin).length()*0.5f);
  Vector3 light_to_origin(scene_middle - lightpos);
  const float distance_to_scene_middle(light_to_origin.length());
  const float nearPlaneZ(distance_to_scene_middle - scene_radius);
  const float farPlaneZ(distance_to_scene_middle + scene_radius);
  const float fov(360.0f*(atan(scene_radius/nearPlaneZ)/math::PI));

  // may be not so good
  Vector3 up(0.0,1.0,0.0);

  if( std::abs(_spotCutoff) > 0.00001)
  {
    /// we assume the _spotCutoff is 1/2 of the spot lights coneangle (in degrees)
    _camera->setFov(_spotCutoff*2.2);
    _camera->setNear(_spotNear);
    _camera->setFar(_spotFar);

    _camera->lookAt(lightpos,
                     gloost::Point3(lightpos[0] + _spotDirection[0],
                                    lightpos[1] + _spotDirection[1],
                                    lightpos[2] + _spotDirection[2]),
                     up);
  }
  else
  {
    _camera->setFov(fov);
    _camera->setNear(nearPlaneZ);
    _camera->setFar(farPlaneZ);

    _camera->lookAt(lightpos,
                     scene_middle,
                     up);
  }



#if 0
  _camera->set();
#endif // 0

  glEnable(GL_DEPTH_TEST);
  glEnable (GL_POLYGON_OFFSET_FILL);
  glPolygonOffset (_polygonOffsetFactor, _polygonOffsetUnits);
}

///////////////////////////////////////////////////////////////////////////////


  ///

void
ShadowMapPointLight::endShadowMapCreation()
{
  // leave fbo
  glPopAttrib();
  _viewport.leave();
  gl::Fbo::unbind();
}
///////////////////////////////////////////////////////////////////////////////


  ///

void
ShadowMapPointLight::applyShadowMapMatrix(const Camera* cam){
  Matrix t;
  t.setIdentity();
  t.setTranslate(0.5,0.5,0.5);
  Matrix s;
  s.setIdentity();
  s.setScale(0.5,0.5,0.5);

  Matrix smap_matrix(cam->getViewMatrix());
  smap_matrix.invert();
  smap_matrix.mult(_camera->getViewMatrix());
  smap_matrix.mult(_camera->getProjectionMatrix());
  smap_matrix.mult(s);
  smap_matrix.mult(t);


  _shadowMapTexture->setTextureMatrix(smap_matrix);
}
///////////////////////////////////////////////////////////////////////////////


  ///

unsigned int
ShadowMapPointLight::getShadowMapId()
{
  return _shadowMapTexture_id;
}


///////////////////////////////////////////////////////////////////////////////


/// set value for glPolygonOffset
void
ShadowMapPointLight::setPolygonOffsetFactor(float factor)
{
  _polygonOffsetFactor = factor;
}



void
ShadowMapPointLight::setPolygonOffsetUnits(float units)
{
  _polygonOffsetUnits = units;
}


///////////////////////////////////////////////////////////////////////////////


  ///


} // namespace gloost
