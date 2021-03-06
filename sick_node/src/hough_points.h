// Generated by gencpp from file sick_node/hough_points.msg
// DO NOT EDIT!


#ifndef SICK_NODE_MESSAGE_HOUGH_POINTS_H
#define SICK_NODE_MESSAGE_HOUGH_POINTS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>
#include <sick_node/hough_point.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Point.h>

namespace sick_node
{
template <class ContainerAllocator>
struct hough_points_
{
  typedef hough_points_<ContainerAllocator> Type;

  hough_points_()
    : header()
    , points()
    , begin()
    , end()  {
    }
  hough_points_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , points(_alloc)
    , begin(_alloc)
    , end(_alloc)  {
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector< ::sick_node::hough_point_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::sick_node::hough_point_<ContainerAllocator> >::other >  _points_type;
  _points_type points;

   typedef  ::geometry_msgs::Point_<ContainerAllocator>  _begin_type;
  _begin_type begin;

   typedef  ::geometry_msgs::Point_<ContainerAllocator>  _end_type;
  _end_type end;




  typedef boost::shared_ptr< ::sick_node::hough_points_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::sick_node::hough_points_<ContainerAllocator> const> ConstPtr;

}; // struct hough_points_

typedef ::sick_node::hough_points_<std::allocator<void> > hough_points;

typedef boost::shared_ptr< ::sick_node::hough_points > hough_pointsPtr;
typedef boost::shared_ptr< ::sick_node::hough_points const> hough_pointsConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::sick_node::hough_points_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::sick_node::hough_points_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace sick_node

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': True}
// {'sick_node': ['/home/sdsuvei/catkin_ws/src/sick_node/msg'], 'geometry_msgs': ['/opt/ros/indigo/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::sick_node::hough_points_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::sick_node::hough_points_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::sick_node::hough_points_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::sick_node::hough_points_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sick_node::hough_points_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sick_node::hough_points_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::sick_node::hough_points_<ContainerAllocator> >
{
  static const char* value()
  {
    return "908aaf3626208debfaaf246dffbbfca1";
  }

  static const char* value(const ::sick_node::hough_points_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x908aaf3626208debULL;
  static const uint64_t static_value2 = 0xfaaf246dffbbfca1ULL;
};

template<class ContainerAllocator>
struct DataType< ::sick_node::hough_points_<ContainerAllocator> >
{
  static const char* value()
  {
    return "sick_node/hough_points";
  }

  static const char* value(const ::sick_node::hough_points_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::sick_node::hough_points_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n\
hough_point[] points\n\
geometry_msgs/Point begin\n\
geometry_msgs/Point end\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: sick_node/hough_point\n\
float32 x\n\
float32 y\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
";
  }

  static const char* value(const ::sick_node::hough_points_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::sick_node::hough_points_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.points);
      stream.next(m.begin);
      stream.next(m.end);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct hough_points_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::sick_node::hough_points_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::sick_node::hough_points_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "points[]" << std::endl;
    for (size_t i = 0; i < v.points.size(); ++i)
    {
      s << indent << "  points[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::sick_node::hough_point_<ContainerAllocator> >::stream(s, indent + "    ", v.points[i]);
    }
    s << indent << "begin: ";
    s << std::endl;
    Printer< ::geometry_msgs::Point_<ContainerAllocator> >::stream(s, indent + "  ", v.begin);
    s << indent << "end: ";
    s << std::endl;
    Printer< ::geometry_msgs::Point_<ContainerAllocator> >::stream(s, indent + "  ", v.end);
  }
};

} // namespace message_operations
} // namespace ros

#endif // SICK_NODE_MESSAGE_HOUGH_POINTS_H
