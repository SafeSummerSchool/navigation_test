/*
 * MyMomentInvariantsEstimation.h
 *
 *  Created on: Feb 12, 2015
 *      Author: mikkel
 */

#ifndef PCL_ROS_SRC_GLOBALPLANEDISTANCE_H_
#define PCL_ROS_SRC_GLOBALPLANEDISTANCE_H_

#include <pcl/features/feature.h>
#include <pcl/common/centroid.h>
#include "obstacle_detection.h"
//#include <pcl/point_types.h>
//#include <pcl/impl/instantiate.hpp>

using namespace pcl;

namespace pcl {

template <typename PointInT, typename PointOutT>
class GlobalPlaneDistance: public Feature<PointInT, PointOutT>
{
public:
//	typedef boost::shared_ptr<MyMomentInvariantsEstimation<PointInT, PointOutT> > Ptr;
//	typedef boost::shared_ptr<const MyMomentInvariantsEstimation<PointInT, PointOutT> > ConstPtr;
	typedef typename Feature<PointInT, PointOutT>::PointCloudOut PointCloudOut;
//	using Feature<PointInT, PointOutT>::feature_name_;

	typedef boost::shared_ptr<GlobalPlaneDistance<PointInT, PointOutT> > Ptr;
	typedef boost::shared_ptr<const GlobalPlaneDistance<PointInT, PointOutT> > ConstPtr;
	using Feature<PointInT, PointOutT>::feature_name_;
	using Feature<PointInT, PointOutT>::getClassName;
	using Feature<PointInT, PointOutT>::indices_;
	using Feature<PointInT, PointOutT>::k_;
	using Feature<PointInT, PointOutT>::search_parameter_;
	using Feature<PointInT, PointOutT>::surface_;
	using Feature<PointInT, PointOutT>::input_;

	/** \brief Empty constructor. */
	GlobalPlaneDistance () : xyz_centroid_ (), temp_pt_ ()
	{
		feature_name_ = "GlobalPlaneDistance";
	};

	/** \brief Estimate moment invariants for all points given in <setInputCloud (), setIndices ()> using the surface
	 * in setSearchSurface () and the spatial locator in setSearchMethod ()
	 * \param[out] output the resultant point cloud model dataset that contains the moment invariants
	 */
	//void
	//computeFeature (PointCloudOut &output);
	void
	computePointMomentInvariants (
	      const pcl::PointCloud<PointInT> &cloud, const std::vector<int> &indices,
	      float &GPDistMean, float &GPDistVar)
	{
	  // Estimate the XYZ centroid
	  compute3DCentroid (cloud, indices, xyz_centroid_);

	  // Initalize the centralized moments
	  float zvar = 0;

	  // Iterate over the nearest neighbors set
	  for (size_t nn_idx = 0; nn_idx < indices.size (); ++nn_idx)
	  {
	    // Demean the points
		  zvar += (cloud.points[indices[nn_idx]].z-xyz_centroid_[2])*(cloud.points[indices[nn_idx]].z-xyz_centroid_[2]);
	  }

	  // Save the moment invariants
	  GPDistMean = xyz_centroid_[2];
	  GPDistVar = zvar/indices.size();
	  //j3 = 0;
	}

protected:
	void
	computeFeature (PointCloudOut &output)
	{
	  // Allocate enough space to hold the results
	  // \note This resize is irrelevant for a radiusSearch ().
	  std::vector<int> nn_indices (k_);
	  std::vector<float> nn_dists (k_);

	  output.is_dense = true;
	  // Save a few cycles by not checking every point for NaN/Inf values if the cloud is set to dense
	  if (input_->is_dense)
	  {
	    // Iterating over the entire index vector
	    for (size_t idx = 0; idx < indices_->size (); ++idx)
	    {
	      if (this->searchForNeighbors ((*indices_)[idx], search_parameter_, nn_indices, nn_dists) == 0)
	      {
	    	  output.points[idx].GPDistMean = output.points[idx].GPDistVar = std::numeric_limits<float>::quiet_NaN ();
	        output.is_dense = false;
	        continue;
	      }

	      computePointMomentInvariants (*surface_, nn_indices,
	                                  output.points[idx].GPDistMean, output.points[idx].GPDistVar);
	    }
	  }
	  else
	  {
	    // Iterating over the entire index vector
	    for (size_t idx = 0; idx < indices_->size (); ++idx)
	    {
	      if (!isFinite ((*input_)[(*indices_)[idx]]) ||
	          this->searchForNeighbors ((*indices_)[idx], search_parameter_, nn_indices, nn_dists) == 0)
	      {
	        output.points[idx].GPDistMean = output.points[idx].GPDistVar = std::numeric_limits<float>::quiet_NaN ();
	        output.is_dense = false;
	        continue;
	      }

	      computePointMomentInvariants (*surface_, nn_indices,
	    		  output.points[idx].GPDistMean, output.points[idx].GPDistVar);
	    }
	  }
	}

private:
      /** \brief 16-bytes aligned placeholder for the XYZ centroid of a surface patch. */
      Eigen::Vector4f xyz_centroid_;

      /** \brief Internal data vector. */
      Eigen::Vector4f temp_pt_;
	//}
};

} /* namespace pcl */

#endif /* PCL_ROS_SRC_GLOBALPLANEDISTANCE_H_ */
