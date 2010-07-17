/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2010  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

#ifndef mrpt_vision_utils_H
#define mrpt_vision_utils_H

#include <mrpt/vision/CFeature.h>
#include <mrpt/utils/CImage.h>
#include <mrpt/math/utils.h>
#include <mrpt/utils/CLoadableOptions.h>
#include <mrpt/utils/TMatchingPair.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/CPoint2D.h>
#include <mrpt/poses/CPoint3D.h>
#include <mrpt/slam/CLandmarksMap.h>
#include <mrpt/slam/CObservationVisualLandmarks.h>

#include <mrpt/vision/types.h>
#include <mrpt/vision/chessboard_camera_calib.h>

#include <mrpt/vision/link_pragmas.h>

namespace mrpt
{
	namespace slam
	{
		class CObservationStereoImages;
		class CObservationBearingRange;
	}

	/** Classes for computer vision, detectors, features, etc.
	 */
	namespace vision
	{
		using namespace mrpt::slam;
		using namespace mrpt::math;
		using namespace mrpt::utils;

			/**	Computes the correlation between this image and another one, encapsulating the openCV function cvMatchTemplate
			*   This implementation reduced computation time.
			* \param patch_img The "patch" image, which must be equal, or smaller than "this" image. This function supports gray-scale (1 channel only) images.
			* \param x_search_ini The "x" coordinate of the search window.
			* \param y_search_ini The "y" coordinate of the search window.
			* \param x_search_size The width of the search window.
			* \param y_search_size The height of the search window.
			* \param x_max The x coordinate where found the maximun cross correlation value.
			* \param y_max The y coordinate where found the maximun cross correlation value
			* \param max_val The maximun value of cross correlation which we can find
			*  Note: By default, the search area is the whole (this) image.
			* \sa cross_correlation
			*/
			void VISION_IMPEXP openCV_cross_correlation(
													const CImage	&img,
													const CImage	&patch_img,
													size_t				&x_max,
													size_t				&y_max,
													double				&max_val,
													int					x_search_ini=-1,
													int					y_search_ini=-1,
													int					x_search_size=-1,
													int					y_search_size=-1);

			/**	Invert an image using OpenCV function
			*
			*/
			void VISION_IMPEXP flip(CImage		&img);

			/** Extract a UNITARY 3D vector in the direction of a 3D point, given from its (x,y) pixels coordinates, and the camera intrinsic coordinates.
			  *  \param x Pixels coordinates, from the top-left corner of the image.
			  *  \param y Pixels coordinates, from the top-left corner of the image.
			  *  \param A The 3x3 intrinsic parameters matrix for the camera.
			  *
			  * \sa buildIntrinsicParamsMatrix, defaultIntrinsicParamsMatrix
			  */
			TPoint3D VISION_IMPEXP pixelTo3D( const vision::TPixelCoordf &xy, const CMatrixDouble33 &A);

			/** Builds the intrinsic parameters matrix A from parameters:
			  * \param focalLengthX The focal length, in X (horizontal) pixels
			  * \param focalLengthY The focal length, in Y (vertical) pixels
			  * \param centerX The image center, horizontal, in pixels
			  * \param centerY The image center, vertical, in pixels
			  *
			  * <br>This method returns the matrix:
			  <table>
			  <tr><td>f_x</td><td>0</td><td>cX</td> </tr>
			  <tr><td>0</td><td>f_y</td><td>cY</td> </tr>
			  <tr><td>0</td><td>0</td><td>1</td> </tr>
			  </table>
			  *  See also the tutorial discussing the <a rhref="http://www.mrpt.org/Camera_Parameters">camera model parameters</a>.
			  * \sa defaultIntrinsicParamsMatrix, pixelTo3D
			  */
			CMatrixDouble33 VISION_IMPEXP buildIntrinsicParamsMatrix(
				const double focalLengthX,
				const double focalLengthY,
				const double centerX,
				const double centerY);

			/** Returns the stored, default intrinsic params matrix for a given camera:
			  * \param camIndex  Posible values are listed next.
			  * \param resolutionX The number of pixel columns
			  * \param resolutionY The number of pixel rows
			  *
			  * The matrix is generated for the indicated camera resolution configuration.
			  * The following table summarizes the current supported cameras and the values as
			  *  ratios of the corresponding horz. or vert. resolution:<br>

			  <center><table>
			  <tr>
			   <td><center><b>camIndex</b></center></td>
			   <td><center><b>Manufacturer</b></center></td>
			   <td><center><b>Camera model</b></center></td>
			   <td><center><b>fx</b></center></td>
			   <td><center><b>fy</b></center></td>
			   <td><center><b>cx</b></center></td>
			   <td><center><b>cy</b></center></td>
			  </tr>

			  <tr>
			   <td><center>0</center></td>
			   <td><center>Point Grey Research</center></td>
			   <td><center>Bumblebee</center></td>
			   <td><center>0.79345</center></td>
			   <td><center>1.05793</center></td>
			   <td><center>0.55662</center></td>
			   <td><center>0.52692</center></td>
			  </tr>

			  <tr>
			   <td><center>1</center></td>
			   <td><center>Sony</center></td>
			   <td><center>???</center></td>
			   <td><center>0.95666094</center></td>
			   <td><center>1.3983423f</center></td>
			   <td><center>0.54626328f</center></td>
			   <td><center>0.4939191f</center></td>
			  </tr>
			  </table>
			  </center>

			  * \sa buildIntrinsicParamsMatrix, pixelTo3D
			  */
			CMatrixDouble33 VISION_IMPEXP defaultIntrinsicParamsMatrix(
				unsigned int camIndex = 0,
				unsigned int resolutionX = 320,
				unsigned int resolutionY = 240 );

			/** Explore the feature list and removes features which are in the same coordinates
			  * \param list (Input). The list of features.
			  */
			void VISION_IMPEXP deleteRepeatedFeats( CFeatureList &list );

			/** Search for correspondences which are not in the same row and deletes them
			  * ...
			  */
			void VISION_IMPEXP rowChecking(	CFeatureList &leftList,
								CFeatureList &rightList,
								float threshold = 0.0);

			/** Computes the dispersion of the features in the image
			  * \param list (IN) Input list of features
			  * \param std	(OUT) 2 element vector containing the standard deviations in the 'x' and 'y' coordinates.
			  * \param mean	(OUT) 2 element vector containing the mean in the 'x' and 'y' coordinates.
			  */
			void VISION_IMPEXP getDispersion( const CFeatureList &list,
											  vector_float &std,
											  vector_float &mean );



			/** Returns a new image where distortion has been removed.
			  * \param A The 3x3 intrinsic parameters matrix
			  * \param dist_coeffs The 1x4 (or 1x5) vector of distortion coefficients
			  */
			inline void correctDistortion(
					const CImage	&in_img,
					CImage			&out_img,
					const CMatrixDouble33	&A,
					const vector_double &dist_coeffs )
			{
				in_img.rectifyImage( out_img, A, dist_coeffs);
			}


			/** Computes the mean squared distance between a set of 3D correspondences
			  * ...
			  */
			double VISION_IMPEXP computeMsd( const mrpt::utils::TMatchingPairList &list,
								const mrpt::poses::CPose3D &Rt );

			/** Transform two clouds of 3D points into a matched list of points
			  * ...
			  */
			void VISION_IMPEXP cloudsToMatchedList( const mrpt::slam::CObservationVisualLandmarks &cloud1,
									   const mrpt::slam::CObservationVisualLandmarks &cloud2,
											 mrpt::utils::TMatchingPairList &outList);

			/** Computes the main orientation of a set of points with an image (for using in SIFT-based algorithms)
			  * \param image (Input). The input image.
			  * \param x (Input). A vector containing the 'x' coordinates of the image points.
			  * \param y (Input). A vector containing the 'y' coordinates of the image points.
			  * \param orientation (Output). A vector containing the main orientation of the image points.
			  */
			float VISION_IMPEXP computeMainOrientation( const CImage &image,
										  const unsigned int &x,
										  const unsigned int &y );

			/** Find the matches between two lists of features. They must be of the same type. Return value: the number of matched pairs of features
			  * \param list1 (Input). One list.
			  * \param list2 (Input). Other list.
			  * \param matches (Output). A vector of pairs of correspondences.
			  * \param options (Optional Input). A struct containing matching options
			  */
			size_t VISION_IMPEXP matchFeatures( const CFeatureList &list1,
								  const CFeatureList &list2,
								  CMatchedFeatureList &matches,
								  const TMatchingOptions &options = TMatchingOptions() );

			/** Find the matches between two lists of features. They must be of the same type. Return value: the number of matched pairs of features
			  * \param list1 (Input). One list.
			  * \param list2 (Input). Other list.
			  * \param matches (Output). A vector of pairs of correspondences.
			  * \param options (Optional Input). A struct containing matching options
			  */
			size_t VISION_IMPEXP matchFeatures2( const CFeatureList &list1,
								  const CFeatureList &list2,
								  CMatchedFeatureList &matches,
								  const TMatchingOptions &options = TMatchingOptions() );


			/** Draw rectangles around each of the features.
			  */
			void VISION_IMPEXP addFeaturesToImage(
				const CImage &inImg,
				const CFeatureList &theList,
				CImage &outImg );

			/** Project a list of matched features into the 3D space, using the provided options for the stereo system
			  * \param matches (Input). The list of matched features.
			  * \param options (Input). The options of the stereo system.
			  * \param landmarks (Output). A map containing the projected landmarks.
			  */
			void VISION_IMPEXP projectMatchedFeatures(
				CMatchedFeatureList					&mfList,		// The set of matched features
				const vision::TStereoSystemParams	&param,			// Parameters for the stereo system
				mrpt::slam::CLandmarksMap			&landmarks );	// Output map of 3D landmarks

			/** Project a pair of feature lists into the 3D space, using the provided options for the stereo system. The matches must be in order,
			  *	i.e. leftList[0] corresponds to rightList[0] and so on;
			  * \param leftList (Input). The left list of matched features.
			  * \param rightList (Input). The right list of matched features.
			  * \param options (Input). The options of the stereo system.
			  * \param landmarks (Output). A map containing the projected landmarks.
			  */
			void VISION_IMPEXP projectMatchedFeatures(
				CFeatureList						&leftList,		// The left of matched features (matches must be ordered!)
				CFeatureList						&rightList,		// The right of matched features (matches must be ordered!)
				const vision::TStereoSystemParams	&param,			// Parameters for the stereo system
				mrpt::slam::CLandmarksMap			&landmarks );	// Output map of 3D landmarks


			/** Converts a stereo images observation into a bearing and range observation.
				\param inObs	[IN]	The input stereo images observation.
				\param sg		[IN]	The sigma of the row, col, and disparity variables involved in the feature detection.
				\param outObs	[OUT]	The output bearing and range observation (including covariances).
			*/
			void VISION_IMPEXP StereoObs2BRObs(
				const CObservationStereoImages &inObs,
				const std::vector<double> &sg,
				CObservationBearingRange &outObs
				);

			/** Converts a matched feature list into a bearing and range observation (some of the stereo camera system must be provided).
				\param inMatches		[IN]	The input list of matched features.
				\param intrinsicParams	[IN]	The intrisic params of the reference (left) camera of the stereo system.
				\param baseline			[IN]	The distance among the X axis of the right camera wrt the reference (left) camera.
				\param sg				[IN]	The sigma of the row, col, and disparity variables involved in the feature detection.
				\param outObs			[OUT]	The output bearing and range observation (including covariances).
			*/
			void VISION_IMPEXP StereoObs2BRObs(
				const CMatchedFeatureList &inMatches,
				const CMatrixDouble33 &intrinsicParams,
				const double &baseline,
				const CPose3D &sensorPose,
				const std::vector<double> &sg,
				CObservationBearingRange &outObs
				);

			/** Converts a CObservationVisualLandmarks into a bearing and range observation (without any covariances). Fields of view are not computed.
				\param inObs			[IN]	The input observation.
				\param outObs			[OUT]	The output bearing and range observation.
			*/
			void VISION_IMPEXP StereoObs2BRObs(
				const CObservationVisualLandmarks &inObs,
				CObservationBearingRange &outObs
				);
	}
}


#endif
