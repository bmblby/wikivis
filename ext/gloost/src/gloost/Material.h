
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


#ifndef H_GLOOST_MATERIAL
#define H_GLOOST_MATERIAL



// gloost includes
#include <gloost/UniformTypes.h>



// cpp includes
#include <string>
#include <map>


namespace gloost
{

  /// Stores different types like float, vec2, vec3, vec4,, mat3, mat4, ... to describe material properties

class Material
{

  public:

    struct CustomResource
    {
      CustomResource():
        _path(""),
        _type(0)
      {

      }

      CustomResource(const std::string& path, unsigned type):
        _path(path),
        _type(type)
      {

      }

      CustomResource(const CustomResource& res):
        _path(res._path),
        _type(res._type)
      {

      }

      std::string _path;
      unsigned    _type;
    };



    /// class constructor
    Material();

    /// class destructor
    virtual ~Material();


    /// Add a float value to the shader
    void  set_float (const std::string& uniformName, float value);
    ///
    float  get_float (const std::string& uniformName);

    /// Add a vec2 to the shader
    void  set_vec2 (const std::string& uniformName, vec2 value);
    ///
    vec2 get_vec2 (const std::string& uniformName);

    /// Add a vec3 to the shader
    void set_vec3 (const std::string& uniformName, vec3 value);
    ///
    vec3 get_vec3 (const std::string& uniformName);

    /// Add a vec4 to the shader
    void set_vec4 (const std::string& uniformName, vec4 value);
    ///
    vec4 get_vec4 (const std::string& uniformName);



    /// Add a integer to the shader
    void set_int(const std::string& uniformName, int value);
    ///
    int get_int(const std::string& uniformName);

    /// Add a ivec2 to the shader
    void  set_ivec2 (const std::string& uniformName, ivec2 value);
    ///
    ivec2 get_ivec2 (const std::string& uniformName);

    /// Add a ivec3 to the shader
    void set_ivec3 (const std::string& uniformName, ivec3 value);
    ///
    ivec3 get_ivec3 (const std::string& uniformName);

    /// Add a ivec4 to the shader
    void set_ivec4 (const std::string& uniformName, ivec4 value);
    ///
    ivec4 get_ivec4 (const std::string& uniformName);



    /// Add a bool to the shader
    void set_bool(const std::string& uniformName, bool value);
    ///
    bool get_bool(const std::string& uniformName);

    /// Add a bvec2 to the shader
    void  set_bvec2 (const std::string& uniformName, bvec2 value);
    ///
    bvec2 get_bvec2 (const std::string& uniformName);

    /// Add a bvec3 to the shader
    void set_bvec3 (const std::string& uniformName, bvec3 value);
    ///
    bvec3 get_bvec3 (const std::string& uniformName);

    /// Add a bvec4 to the shader
    void set_bvec4 (const std::string& uniformName, bvec4 value);
    ///
    bvec4 get_bvec4 (const std::string& uniformName);



    /// Add a mat2 to the shader
    void set_mat2 (const std::string& uniformName, mat2 value);
    ///
    mat2 get_mat2 (const std::string& uniformName);

    /// Add a mat3 to the shader
    void set_mat3 (const std::string& uniformName, mat3 value);
    ///
    mat3 get_mat3 (const std::string& uniformName);

    /// Add a mat4 to the shader
    void set_mat4 (const std::string& uniformName, mat4 value);
    ///
    mat4 get_mat4 (const std::string& uniformName);


    /// Add a sampler to the shader (this is the id from the gloost::TextureManager)
    void set_sampler (const std::string& uniformName, gloostId textureGid);
    ///
    gloostId get_sampler (const std::string& uniformName);


    // add a string
    void set_string(const std::string& name, const std::string& value);

    std::string get_string(const std::string& name);

    // add a custom resource
    void set_customResource(const std::string& name, const CustomResource& value);
    void set_customResource(const std::string& name, const std::string& path, unsigned type);

    CustomResource get_customResource(const std::string& name);


  protected:


    // Uniforms
    std::map<std::string, float>     _map_float;
    std::map<std::string, vec2>      _map_vec2;
    std::map<std::string, vec3>      _map_vec3;
    std::map<std::string, vec4>      _map_vec4;

    std::map<std::string, int>       _map_int;
    std::map<std::string, ivec2>     _map_ivec2;
    std::map<std::string, ivec3>     _map_ivec3;
    std::map<std::string, ivec4>     _map_ivec4;

    std::map<std::string, bool>      _map_bool;
    std::map<std::string, bvec2>     _map_bvec2;
    std::map<std::string, bvec3>     _map_bvec3;
    std::map<std::string, bvec4>     _map_bvec4;

    std::map<std::string, mat2>      _map_mat2;
    std::map<std::string, mat3>      _map_mat3;
    std::map<std::string, mat4>      _map_mat4;

    std::map<std::string, gloostId>  _map_sampler;

    // additional types
    std::map<std::string, std::string>    _map_string;
    std::map<std::string, CustomResource> _map_customResources;

};


} // namespace gloost


#endif // H_GLOOST_MATERIAL
