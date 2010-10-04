cmake_minimum_required(VERSION 2.8)
project(copo-tracer)

include_directories(
    /usr/include/
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	src/
    src/common
)

link_directories(
	#dependencies/fltk/lib
)

#add_definitions(-DAPIENTRY=/**/)

set(COPOTRACER_H
    src/raytracer/raytracer.h
    src/raytracer/plane.h
    src/raytracer/sphere.h
    src/raytracer/camera.h
    src/raytracer/blinn_phong.h
    src/raytracer/phong.h
    src/raytracer/light_source.h
    src/raytracer/utils.h
    src/raytracer/transformation_group.h
    src/raytracer/vision_camera.h
    src/raytracer/directional_light.h
    src/raytracer/intersection_info.h
    src/raytracer/spot_light.h
    src/raytracer/point_light.h
    src/raytracer/primitive_group.h
    src/raytracer/ray.h
    src/raytracer/triangle.h
    src/raytracer/obj_reader.h
    src/raytracer/projector_light.h
    src/raytracer/box.h
    src/raytracer/material.h
    src/raytracer/cylinder.h
    src/raytracer/kd_tree.h
    src/raytracer/scene.h
    src/raytracer/cg_camera.h
    src/raytracer/primitive.h
    src/common/progress.h
    src/common/union_find.h
    src/common/checkerboard_pattern.h
    src/common/phaseshift_pattern.h
    src/common/variational_graycode.h
    src/common/tiny_vec.h
    src/common/image_inpainting.h
    src/common/timing.h
    src/common/image_utils.h
    src/common/math_utils.h
    src/common/image_denoising.h
    src/common/graycode_pattern.h
    src/common/random_utils.h
    src/common/triangle_pattern.h
    src/common/on_off_pattern.h
    src/common/vec.h
    src/common/sampler_2d.h
    src/common/mat.h
    src/common/svd.h
    src/common/tiny_mat.h
    src/common/rasterizer_2d.h
    src/common/file_utils.h
    src/common/colorize.h
    src/common/image_io.h
)

set(COPOTRACER_CPP
    src/raytracer/intersection_info.cxx
    src/raytracer/spot_light.cxx
    src/raytracer/kd_tree.cxx
    src/raytracer/plane.cxx
    src/raytracer/blinn_phong.cxx
    src/raytracer/light_source.cxx
    src/raytracer/primitive.cxx
    src/raytracer/vision_camera.cxx
    src/raytracer/cg_camera.cxx
    src/raytracer/camera.cxx
    src/raytracer/point_light.cxx
    src/raytracer/material.cxx
    src/raytracer/phong.cxx
    src/raytracer/raytracer.cxx
    src/raytracer/primitive_group.cxx
    src/raytracer/cylinder.cxx
    src/raytracer/triangle.cxx
    src/raytracer/sphere.cxx
    src/raytracer/projector_light.cxx
    src/raytracer/box.cxx
    src/raytracer/directional_light.cxx
    src/raytracer/transformation_group.cxx
    src/raytracer/scene.cxx
)

set(COPOTRACER_SAMPLE_PROJECTOR_CPP
    samples/projector/projector.cxx
)

set(COPOTRACER_SAMPLE_SIMPLE_CPP
    samples/simple/simple.cxx
)

# The actual raytracer library
add_library(copo-tracer
	${COPOTRACER_CPP}
	${COPOTRACER_H}
)

# A simple sample, produces a normal raytrced image
# and its depth map.
add_executable(sample_simple
	${COPOTRACER_H}
    ${COPOTRACER_SAMPLE_SIMPLE_CPP}
)

target_link_libraries(sample_simple copo-tracer)

# A sample which includes a projector looking from
# above the scene. A sequence is produced and iIt
# shows variing patterns.
add_executable(sample_projector
	${COPOTRACER_H}
    ${COPOTRACER_SAMPLE_PROJECTOR_CPP}
)

target_link_libraries(sample_projector copo-tracer)

