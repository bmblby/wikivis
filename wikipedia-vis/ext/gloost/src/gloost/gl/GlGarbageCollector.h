
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



#ifndef H_GLOOST_GL_GLGARBAGECOLLECTOR
#define H_GLOOST_GL_GLGARBAGECOLLECTOR

// gloost system includes
#include <gloost/gloostConfig.h>
//#include <gloost/gloostMath.h>

// cpp includes
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <thread>
#include <set>


namespace gloost
{
namespace gl
{
  class Fbo;
  class GBuffer;
  class MultiGlContextEntity;
  class Ibo;
  class Vbo;
  class Tbo;
  class Texture;


  //  Stores gl entities to deferre their destruction until a defined gl state is guaranteed

class GlGarbageCollector
{
	public:

    /// a std::shared_ptr of an GlGarbageCollector instance
    typedef std::shared_ptr<GlGarbageCollector>       shared_ptr;
    typedef std::shared_ptr<const GlGarbageCollector> const_shared_ptr;

    // class destructor
	  virtual ~GlGarbageCollector();

	  // adds an gl::Fbo to dispose of
    static void put(const std::shared_ptr<Fbo>& fbo);
	  // adds an gl::GBuffer to dispose of
    static void put(const std::shared_ptr<GBuffer>& gBuffer);
	  // adds an gl::MultiGlContextEntity like gl::Ibo,  gl::Vbo,  gl::Tbo, gl::Texture, ... to dispose of
    static void put(const std::shared_ptr<MultiGlContextEntity>& glEntity);
	  // adds an texture gid to dispose of
    static void putTextureGid(gloostId textureGid);

    // clears all entities from the garbage (requires gl context)
    static void flush();

	protected:

    // class constructor
    GlGarbageCollector();

    // class factory
    static const std::shared_ptr<GlGarbageCollector>& get();

    //
    std::shared_ptr<std::lock_guard<std::mutex> > getLockGuard() const;


	private:

   static shared_ptr  _instance;
   mutable std::mutex _mutex;

   std::set<std::shared_ptr<Fbo> >                  _fbos;
   std::set<std::shared_ptr<GBuffer> >              _gBuffers;
   std::set<std::shared_ptr<MultiGlContextEntity> > _multiGlEntities;
   std::set<gloostId>                               _textureGids;

};


} // namespace gl
} // namespace gloost


#endif // H_GLOOST_GL_GLGARBAGECOLLECTOR


