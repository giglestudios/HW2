// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    //mat3 ret;
    //double radians = degrees * pi/180.0 ;
    double radians = glm::radians(degrees);
    
    glm::mat3 myIdentity = glm::mat3(
                                     1.0f    ,   0.0f    ,   0.0f    ,
                                     0.0f    ,   1.0f    ,   0.0f    ,
                                     0.0f    ,   0.0f    ,   1.0f
                                     
                                     );
    
    //axis into axis multiplatication
    glm::mat3 axisSquare = glm::mat3(
                                     
                                     (axis.x * axis.x) , (axis.x * axis.y) , (axis.x * axis.z) ,
                                     (axis.y * axis.x) , (axis.y * axis.y) , (axis.y * axis.z) ,
                                     (axis.z * axis.x) , (axis.z * axis.y) , (axis.z * axis.z)
                                     
                                     );
    
    glm::mat3 aStar = glm::mat3(
                                0.0f    ,   axis.z      ,   -axis.y ,
                                -axis.z ,   0.0f        ,   axis.x  ,
                                axis.y  ,   -axis.x     ,   0.0f
                                );
    
    
    
    //Angle Axis Rotation formula (Rodrigues rotation formula)
    mat3 rotation =
    ( cos(radians) * myIdentity )
    + ( ( 1 - cos(radians) ) * axisSquare )
    + ( sin(radians) * aStar );
    
    return rotation;
    
    
  //return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    glm::vec3 rotationAxis = glm::normalize(up);
    
    glm::mat3 newRotation = rotate(degrees, rotationAxis);
    
    // Rotating Eye
    eye = newRotation * eye;
    
    //Rotating Up
    up = newRotation * up;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    //New X axis is found through cross product of Z axis (eye) and Y axis (up).
    vec3 rotationAxis = glm::normalize ( glm::cross(eye,up) );
    
    // Rotating Eye
    mat3 newRotation = rotate(degrees, rotationAxis);
    
    eye = newRotation * eye;
    
    //Rotating Up
    up = newRotation * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    //glm::vec3 center = glm::vec3(0.0,0.0,0.0);
    ///const vec3 center(0.0,0.0,0.0);
    
    // Camera Direction Z axis
    glm::vec3 w = glm::normalize( center - eye );
    
    // Camera UP or Y axis of camera space
    glm::vec3 u =  glm::normalize(up);
    
    // Camera Right or X axis
    glm::vec3 v = glm::normalize ( glm::cross(w,u) );
    
    // Actual Y Axis of camera
    u =  glm::cross(v,w);
    
    // Column Major ordering
    glm::mat4 p = glm::mat4(
                            v.x ,   u.x ,   -w.x ,  0.0f
                            ,  v.y ,   u.y ,   -w.y ,  0.0f
                            ,  v.z ,   u.z ,   -w.z ,  0.0f
                            ,   -( glm::dot(v,eye) )   ,   -( glm::dot(u,eye) )   ,   ( glm::dot(w,eye) )   ,   1.0f   );
    
    
    return p;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.
    
    float d = 1/tan(glm::radians(fovy/2));
    
    float a = -(zFar + zNear) / (zFar - zNear);
    
    float b = -(2 * zFar * zNear) / (zFar - zNear);
    
    mat4 ret = mat4(
                        (d/aspect), 0,      0,      0,
                        0,          d,      0,      0,
                        0,          0,      a,     -1,
                        0,          0,      b,      0
                    );
    
    
    return ret;
    
    
    //New Formaula for Perspective
//    glm::float_t range = tan(glm::radians(fovy / 2.0)) * zNear;
//    glm::float_t left = -range * aspect;
//    glm::float_t right = range * aspect;
//    glm::float_t bottom = -range;
//    glm::float_t top = range;
//    
//    mat4 ret(0.0);
//    
//    ret[0][0] = (2.0 * zNear) / (right - left);
//    ret[1][1] = (2.0 * zNear) / (top - bottom);
//    ret[2][2] = - (zFar + zNear) / (zFar - zNear);
//    ret[2][3] = - 1.0;
//    ret[3][2] = - (2.0 * zFar * zNear) / (zFar - zNear);
//    
//    return ret;
    
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    // YOUR CODE FOR HW2 HERE
    // Implement scaling
    
    mat4 ret = mat4(
                        sx, 0,  0,  0,
                        0,  sy, 0,  0,
                        0,  0,  sz, 0,
                        0,  0,  0,  1
    
                    );
    
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  // YOUR CODE FOR HW2 HERE
  // Implement translation
    
    //Might need to transpose in readfile
    mat4 ret = mat4(
                        1,  0,  0,  0,
                        0,  1,  0,  0,
                        0,  0,  1,  0,
                        tx, ty, tz, 1
                    );
    
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{

    // VINAYAK
    // Direction Z axis
    glm::vec3 w = glm::normalize( zvec );
    
    //  UP or Y axis
    glm::vec3 u =  glm::normalize(up);
    
    //  Right or X axis
    glm::vec3 v = glm::normalize ( glm::cross(w,u) );
    
    // Actual Y Axis
    return glm::cross(v,w);;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
