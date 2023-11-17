cmake_minimum_required(VERSION 3.5)

project(scara3D VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)

#file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

set(MAIN_FILES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    src/framelesswindow.cpp src/framelesswindow.h
    src/logger.cpp src/logger.h
)

set(OPENGL_FILES
    src/model.cpp src/model.h
    src/modelimport.cpp src/modelimport.h
    src/mesh.cpp src/mesh.h
    src/vao.cpp src/vao.h
    src/vbo.cpp src/vbo.h
    src/ebo.cpp src/ebo.h
    src/vertex.cpp src/vertex.h
    src/boundary.cpp src/boundary.h
    src/camera.cpp src/camera.h
    src/ray.h
    src/hitrecord.h
    src/illuminer.cpp src/illuminer.h
    src/shader.cpp src/shader.h
    src/renderable.cpp src/renderable.h
    src/utils.h
    src/qopenglext.h
    src/skybox.cpp src/skybox.h
    src/terrain.cpp src/terrain.h
    src/texture.cpp src/texture.h
)
set(MODEL_FILES
    src/modelattrslide.cpp src/modelattrslide.h
#    src/modelselectable.cpp src/modelselectable.h
#    src/modelselector.cpp src/modelselector.h
    src/modelsetter.cpp src/modelsetter.h
#    src/modelthumbnailwidget.cpp src/modelthumbnailwidget.h
)
set(INCLUDE_FILES
include/GLM/detail/_features.hpp include/GLM/detail/_fixes.hpp include/GLM/detail/_noise.hpp include/GLM/detail/_swizzle.hpp include/GLM/detail/_swizzle_func.hpp include/GLM/detail/_vectorize.hpp include/GLM/detail/compute_common.hpp include/GLM/detail/compute_vector_relational.hpp include/GLM/detail/func_common.inl include/GLM/detail/func_common_simd.inl include/GLM/detail/func_exponential.inl include/GLM/detail/func_exponential_simd.inl include/GLM/detail/func_geometric.inl include/GLM/detail/func_geometric_simd.inl include/GLM/detail/func_integer.inl include/GLM/detail/func_integer_simd.inl include/GLM/detail/func_matrix.inl include/GLM/detail/func_matrix_simd.inl include/GLM/detail/func_packing.inl include/GLM/detail/func_packing_simd.inl include/GLM/detail/func_trigonometric.inl include/GLM/detail/func_trigonometric_simd.inl include/GLM/detail/func_vector_relational.inl include/GLM/detail/func_vector_relational_simd.inl include/GLM/detail/glm.cpp include/GLM/detail/qualifier.hpp include/GLM/detail/setup.hpp include/GLM/detail/type_float.hpp include/GLM/detail/type_half.hpp include/GLM/detail/type_half.inl include/GLM/detail/type_mat2x2.hpp include/GLM/detail/type_mat2x2.inl include/GLM/detail/type_mat2x3.hpp include/GLM/detail/type_mat2x3.inl include/GLM/detail/type_mat2x4.hpp include/GLM/detail/type_mat2x4.inl include/GLM/detail/type_mat3x2.hpp include/GLM/detail/type_mat3x2.inl include/GLM/detail/type_mat3x3.hpp include/GLM/detail/type_mat3x3.inl include/GLM/detail/type_mat3x4.hpp include/GLM/detail/type_mat3x4.inl include/GLM/detail/type_mat4x2.hpp include/GLM/detail/type_mat4x2.inl include/GLM/detail/type_mat4x3.hpp include/GLM/detail/type_mat4x3.inl include/GLM/detail/type_mat4x4.hpp include/GLM/detail/type_mat4x4.inl include/GLM/detail/type_mat4x4_simd.inl include/GLM/detail/type_quat.hpp include/GLM/detail/type_quat.inl include/GLM/detail/type_quat_simd.inl include/GLM/detail/type_vec1.hpp include/GLM/detail/type_vec1.inl include/GLM/detail/type_vec2.hpp include/GLM/detail/type_vec2.inl include/GLM/detail/type_vec3.hpp include/GLM/detail/type_vec3.inl include/GLM/detail/type_vec4.hpp include/GLM/detail/type_vec4.inl include/GLM/detail/type_vec4_simd.inl include/GLM/ext/matrix_clip_space.hpp include/GLM/ext/matrix_clip_space.inl include/GLM/ext/matrix_common.hpp include/GLM/ext/matrix_common.inl include/GLM/ext/matrix_double2x2.hpp include/GLM/ext/matrix_double2x2_precision.hpp include/GLM/ext/matrix_double2x3.hpp include/GLM/ext/matrix_double2x3_precision.hpp include/GLM/ext/matrix_double2x4.hpp include/GLM/ext/matrix_double2x4_precision.hpp include/GLM/ext/matrix_double3x2.hpp include/GLM/ext/matrix_double3x2_precision.hpp include/GLM/ext/matrix_double3x3.hpp include/GLM/ext/matrix_double3x3_precision.hpp include/GLM/ext/matrix_double3x4.hpp include/GLM/ext/matrix_double3x4_precision.hpp include/GLM/ext/matrix_double4x2.hpp include/GLM/ext/matrix_double4x2_precision.hpp include/GLM/ext/matrix_double4x3.hpp include/GLM/ext/matrix_double4x3_precision.hpp include/GLM/ext/matrix_double4x4.hpp include/GLM/ext/matrix_double4x4_precision.hpp include/GLM/ext/matrix_float2x2.hpp include/GLM/ext/matrix_float2x2_precision.hpp include/GLM/ext/matrix_float2x3.hpp include/GLM/ext/matrix_float2x3_precision.hpp include/GLM/ext/matrix_float2x4.hpp include/GLM/ext/matrix_float2x4_precision.hpp include/GLM/ext/matrix_float3x2.hpp include/GLM/ext/matrix_float3x2_precision.hpp include/GLM/ext/matrix_float3x3.hpp include/GLM/ext/matrix_float3x3_precision.hpp include/GLM/ext/matrix_float3x4.hpp include/GLM/ext/matrix_float3x4_precision.hpp include/GLM/ext/matrix_float4x2.hpp include/GLM/ext/matrix_float4x2_precision.hpp include/GLM/ext/matrix_float4x3.hpp include/GLM/ext/matrix_float4x3_precision.hpp include/GLM/ext/matrix_float4x4.hpp include/GLM/ext/matrix_float4x4_precision.hpp include/GLM/ext/matrix_int2x2.hpp include/GLM/ext/matrix_int2x2_sized.hpp include/GLM/ext/matrix_int2x3.hpp include/GLM/ext/matrix_int2x3_sized.hpp include/GLM/ext/matrix_int2x4.hpp include/GLM/ext/matrix_int2x4_sized.hpp include/GLM/ext/matrix_int3x2.hpp include/GLM/ext/matrix_int3x2_sized.hpp include/GLM/ext/matrix_int3x3.hpp include/GLM/ext/matrix_int3x3_sized.hpp include/GLM/ext/matrix_int3x4.hpp include/GLM/ext/matrix_int3x4_sized.hpp include/GLM/ext/matrix_int4x2.hpp include/GLM/ext/matrix_int4x2_sized.hpp include/GLM/ext/matrix_int4x3.hpp include/GLM/ext/matrix_int4x3_sized.hpp include/GLM/ext/matrix_int4x4.hpp include/GLM/ext/matrix_int4x4_sized.hpp include/GLM/ext/matrix_projection.hpp include/GLM/ext/matrix_projection.inl include/GLM/ext/matrix_relational.hpp include/GLM/ext/matrix_relational.inl include/GLM/ext/matrix_transform.hpp include/GLM/ext/matrix_transform.inl include/GLM/ext/matrix_uint2x2.hpp include/GLM/ext/matrix_uint2x2_sized.hpp include/GLM/ext/matrix_uint2x3.hpp include/GLM/ext/matrix_uint2x3_sized.hpp include/GLM/ext/matrix_uint2x4.hpp include/GLM/ext/matrix_uint2x4_sized.hpp include/GLM/ext/matrix_uint3x2.hpp include/GLM/ext/matrix_uint3x2_sized.hpp include/GLM/ext/matrix_uint3x3.hpp include/GLM/ext/matrix_uint3x3_sized.hpp include/GLM/ext/matrix_uint3x4.hpp include/GLM/ext/matrix_uint3x4_sized.hpp include/GLM/ext/matrix_uint4x2.hpp include/GLM/ext/matrix_uint4x2_sized.hpp include/GLM/ext/matrix_uint4x3.hpp include/GLM/ext/matrix_uint4x3_sized.hpp include/GLM/ext/matrix_uint4x4.hpp include/GLM/ext/matrix_uint4x4_sized.hpp include/GLM/ext/quaternion_common.hpp include/GLM/ext/quaternion_common.inl include/GLM/ext/quaternion_common_simd.inl include/GLM/ext/quaternion_double.hpp include/GLM/ext/quaternion_double_precision.hpp include/GLM/ext/quaternion_exponential.hpp include/GLM/ext/quaternion_exponential.inl include/GLM/ext/quaternion_float.hpp include/GLM/ext/quaternion_float_precision.hpp include/GLM/ext/quaternion_geometric.hpp include/GLM/ext/quaternion_geometric.inl include/GLM/ext/quaternion_relational.hpp include/GLM/ext/quaternion_relational.inl include/GLM/ext/quaternion_transform.hpp include/GLM/ext/quaternion_transform.inl include/GLM/ext/quaternion_trigonometric.hpp include/GLM/ext/quaternion_trigonometric.inl include/GLM/ext/scalar_common.hpp include/GLM/ext/scalar_common.inl include/GLM/ext/scalar_constants.hpp include/GLM/ext/scalar_constants.inl include/GLM/ext/scalar_int_sized.hpp include/GLM/ext/scalar_integer.hpp include/GLM/ext/scalar_integer.inl include/GLM/ext/scalar_packing.hpp include/GLM/ext/scalar_packing.inl include/GLM/ext/scalar_relational.hpp include/GLM/ext/scalar_relational.inl include/GLM/ext/scalar_uint_sized.hpp include/GLM/ext/scalar_ulp.hpp include/GLM/ext/scalar_ulp.inl include/GLM/ext/vector_bool1.hpp include/GLM/ext/vector_bool1_precision.hpp include/GLM/ext/vector_bool2.hpp include/GLM/ext/vector_bool2_precision.hpp include/GLM/ext/vector_bool3.hpp include/GLM/ext/vector_bool3_precision.hpp include/GLM/ext/vector_bool4.hpp include/GLM/ext/vector_bool4_precision.hpp include/GLM/ext/vector_common.hpp include/GLM/ext/vector_common.inl include/GLM/ext/vector_double1.hpp include/GLM/ext/vector_double1_precision.hpp include/GLM/ext/vector_double2.hpp include/GLM/ext/vector_double2_precision.hpp include/GLM/ext/vector_double3.hpp include/GLM/ext/vector_double3_precision.hpp include/GLM/ext/vector_double4.hpp include/GLM/ext/vector_double4_precision.hpp include/GLM/ext/vector_float1.hpp include/GLM/ext/vector_float1_precision.hpp include/GLM/ext/vector_float2.hpp include/GLM/ext/vector_float2_precision.hpp include/GLM/ext/vector_float3.hpp include/GLM/ext/vector_float3_precision.hpp include/GLM/ext/vector_float4.hpp include/GLM/ext/vector_float4_precision.hpp include/GLM/ext/vector_int1.hpp include/GLM/ext/vector_int1_sized.hpp include/GLM/ext/vector_int2.hpp include/GLM/ext/vector_int2_sized.hpp include/GLM/ext/vector_int3.hpp include/GLM/ext/vector_int3_sized.hpp include/GLM/ext/vector_int4.hpp include/GLM/ext/vector_int4_sized.hpp include/GLM/ext/vector_integer.hpp include/GLM/ext/vector_integer.inl include/GLM/ext/vector_packing.hpp include/GLM/ext/vector_packing.inl include/GLM/ext/vector_relational.hpp include/GLM/ext/vector_relational.inl include/GLM/ext/vector_uint1.hpp include/GLM/ext/vector_uint1_sized.hpp include/GLM/ext/vector_uint2.hpp include/GLM/ext/vector_uint2_sized.hpp include/GLM/ext/vector_uint3.hpp include/GLM/ext/vector_uint3_sized.hpp include/GLM/ext/vector_uint4.hpp include/GLM/ext/vector_uint4_sized.hpp include/GLM/ext/vector_ulp.hpp include/GLM/ext/vector_ulp.inl include/GLM/gtc/bitfield.hpp include/GLM/gtc/bitfield.inl include/GLM/gtc/color_space.hpp include/GLM/gtc/color_space.inl include/GLM/gtc/constants.hpp include/GLM/gtc/constants.inl include/GLM/gtc/epsilon.hpp include/GLM/gtc/epsilon.inl include/GLM/gtc/integer.hpp include/GLM/gtc/integer.inl include/GLM/gtc/matrix_access.hpp include/GLM/gtc/matrix_access.inl include/GLM/gtc/matrix_integer.hpp include/GLM/gtc/matrix_inverse.hpp include/GLM/gtc/matrix_inverse.inl include/GLM/gtc/matrix_transform.hpp include/GLM/gtc/matrix_transform.inl include/GLM/gtc/noise.hpp include/GLM/gtc/noise.inl include/GLM/gtc/packing.hpp include/GLM/gtc/packing.inl include/GLM/gtc/quaternion.hpp include/GLM/gtc/quaternion.inl include/GLM/gtc/quaternion_simd.inl include/GLM/gtc/random.hpp include/GLM/gtc/random.inl include/GLM/gtc/reciprocal.hpp include/GLM/gtc/reciprocal.inl include/GLM/gtc/round.hpp include/GLM/gtc/round.inl include/GLM/gtc/type_aligned.hpp include/GLM/gtc/type_precision.hpp include/GLM/gtc/type_precision.inl include/GLM/gtc/type_ptr.hpp include/GLM/gtc/type_ptr.inl include/GLM/gtc/ulp.hpp include/GLM/gtc/ulp.inl include/GLM/gtc/vec1.hpp include/GLM/gtx/associated_min_max.hpp include/GLM/gtx/associated_min_max.inl include/GLM/gtx/bit.hpp include/GLM/gtx/bit.inl include/GLM/gtx/closest_point.hpp include/GLM/gtx/closest_point.inl include/GLM/gtx/color_encoding.hpp include/GLM/gtx/color_encoding.inl include/GLM/gtx/color_space.hpp include/GLM/gtx/color_space.inl include/GLM/gtx/color_space_YCoCg.hpp include/GLM/gtx/color_space_YCoCg.inl include/GLM/gtx/common.hpp include/GLM/gtx/common.inl include/GLM/gtx/compatibility.hpp include/GLM/gtx/compatibility.inl include/GLM/gtx/component_wise.hpp include/GLM/gtx/component_wise.inl include/GLM/gtx/dual_quaternion.hpp include/GLM/gtx/dual_quaternion.inl include/GLM/gtx/easing.hpp include/GLM/gtx/easing.inl include/GLM/gtx/euler_angles.hpp include/GLM/gtx/euler_angles.inl include/GLM/gtx/extend.hpp include/GLM/gtx/extend.inl include/GLM/gtx/extended_min_max.hpp include/GLM/gtx/extended_min_max.inl include/GLM/gtx/exterior_product.hpp include/GLM/gtx/exterior_product.inl include/GLM/gtx/fast_exponential.hpp include/GLM/gtx/fast_exponential.inl include/GLM/gtx/fast_square_root.hpp include/GLM/gtx/fast_square_root.inl include/GLM/gtx/fast_trigonometry.hpp include/GLM/gtx/fast_trigonometry.inl include/GLM/gtx/float_notmalize.inl include/GLM/gtx/functions.hpp include/GLM/gtx/functions.inl include/GLM/gtx/gradient_paint.hpp include/GLM/gtx/gradient_paint.inl include/GLM/gtx/handed_coordinate_space.hpp include/GLM/gtx/handed_coordinate_space.inl include/GLM/gtx/hash.hpp include/GLM/gtx/hash.inl include/GLM/gtx/integer.hpp include/GLM/gtx/integer.inl include/GLM/gtx/intersect.hpp include/GLM/gtx/intersect.inl include/GLM/gtx/io.hpp include/GLM/gtx/io.inl include/GLM/gtx/log_base.hpp include/GLM/gtx/log_base.inl include/GLM/gtx/matrix_cross_product.hpp include/GLM/gtx/matrix_cross_product.inl include/GLM/gtx/matrix_decompose.hpp include/GLM/gtx/matrix_decompose.inl include/GLM/gtx/matrix_factorisation.hpp include/GLM/gtx/matrix_factorisation.inl include/GLM/gtx/matrix_interpolation.hpp include/GLM/gtx/matrix_interpolation.inl include/GLM/gtx/matrix_major_storage.hpp include/GLM/gtx/matrix_major_storage.inl include/GLM/gtx/matrix_operation.hpp include/GLM/gtx/matrix_operation.inl include/GLM/gtx/matrix_query.hpp include/GLM/gtx/matrix_query.inl include/GLM/gtx/matrix_transform_2d.hpp include/GLM/gtx/matrix_transform_2d.inl include/GLM/gtx/mixed_product.hpp include/GLM/gtx/mixed_product.inl include/GLM/gtx/norm.hpp include/GLM/gtx/norm.inl include/GLM/gtx/normal.hpp include/GLM/gtx/normal.inl include/GLM/gtx/normalize_dot.hpp include/GLM/gtx/normalize_dot.inl include/GLM/gtx/number_precision.hpp include/GLM/gtx/number_precision.inl include/GLM/gtx/optimum_pow.hpp include/GLM/gtx/optimum_pow.inl include/GLM/gtx/orthonormalize.hpp include/GLM/gtx/orthonormalize.inl include/GLM/gtx/perpendicular.hpp include/GLM/gtx/perpendicular.inl include/GLM/gtx/polar_coordinates.hpp include/GLM/gtx/polar_coordinates.inl include/GLM/gtx/projection.hpp include/GLM/gtx/projection.inl include/GLM/gtx/quaternion.hpp include/GLM/gtx/quaternion.inl include/GLM/gtx/range.hpp include/GLM/gtx/raw_data.hpp include/GLM/gtx/raw_data.inl include/GLM/gtx/rotate_normalized_axis.hpp include/GLM/gtx/rotate_normalized_axis.inl include/GLM/gtx/rotate_vector.hpp include/GLM/gtx/rotate_vector.inl include/GLM/gtx/scalar_multiplication.hpp include/GLM/gtx/scalar_relational.hpp include/GLM/gtx/scalar_relational.inl include/GLM/gtx/spline.hpp include/GLM/gtx/spline.inl include/GLM/gtx/std_based_type.hpp include/GLM/gtx/std_based_type.inl include/GLM/gtx/string_cast.hpp include/GLM/gtx/string_cast.inl include/GLM/gtx/texture.hpp include/GLM/gtx/texture.inl include/GLM/gtx/transform.hpp include/GLM/gtx/transform.inl include/GLM/gtx/transform2.hpp include/GLM/gtx/transform2.inl include/GLM/gtx/type_aligned.hpp include/GLM/gtx/type_aligned.inl include/GLM/gtx/type_trait.hpp include/GLM/gtx/type_trait.inl include/GLM/gtx/vec_swizzle.hpp include/GLM/gtx/vector_angle.hpp include/GLM/gtx/vector_angle.inl include/GLM/gtx/vector_query.hpp include/GLM/gtx/vector_query.inl include/GLM/gtx/wrap.hpp include/GLM/gtx/wrap.inl include/GLM/simd/common.h include/GLM/simd/exponential.h include/GLM/simd/geometric.h include/GLM/simd/integer.h include/GLM/simd/matrix.h include/GLM/simd/neon.h include/GLM/simd/packing.h include/GLM/simd/platform.h include/GLM/simd/trigonometric.h include/GLM/simd/vector_relational.h include/GLM/common.hpp include/GLM/exponential.hpp include/GLM/ext.hpp include/GLM/fwd.hpp include/GLM/geometric.hpp include/GLM/glm.hpp include/GLM/integer.hpp include/GLM/mat2x2.hpp include/GLM/mat2x3.hpp include/GLM/mat2x4.hpp include/GLM/mat3x2.hpp include/GLM/mat3x3.hpp include/GLM/mat3x4.hpp include/GLM/mat4x2.hpp include/GLM/mat4x3.hpp include/GLM/mat4x4.hpp include/GLM/matrix.hpp include/GLM/packing.hpp include/GLM/trigonometric.hpp include/GLM/vec2.hpp include/GLM/vec3.hpp include/GLM/vec4.hpp include/GLM/vector_relational.hpp
    include/STBImage/stb_image.h
    #include/pugixml/pugiconfig.hpp include/pugixml/pugixml.cpp include/pugixml/pugixml.hpp
)

set(UI_PAGES_FILES
    src/aboutpage.cpp src/aboutpage.h
    #src/editorpage.cpp src/editorpage.h
    #src/settingpage.cpp src/settingpage.h
    #src/sceneviewer.cpp src/sceneviewer.h#这一个控件依赖着所有opengl的东西
)

set(UI_CONTROL_FILES
    src/sidebar.cpp src/sidebar.h
    src/pushbutton.cpp src/pushbutton.h
    src/pagewidget.cpp src/pagewidget.h
    src/scrolllistwidget.cpp src/scrolllistwidget.h
    src/lineeditwidget.cpp src/lineeditwidget.h
    src/roundedcornerwidget.cpp src/roundedcornerwidget.h
)

set(OTHER_FILES
    Asset/obj/.mynote.txt Asset/obj/base_part.mtl Asset/obj/first_arm.mtl Asset/obj/second_arm.mtl Asset/obj/test.mtl Asset/skybox/back.jpg Asset/skybox/bottom.jpg Asset/skybox/front.jpg Asset/skybox/left.jpg Asset/skybox/right.jpg Asset/skybox/top.jpg Asset/Terrains/grass/heightmap.png Asset/Terrains/grass/texture.jpg Asset/Terrains/metal/heightmap.png Asset/Terrains/metal/texture.jpg Asset/Terrains/rock/heightmap.png Asset/Terrains/rock/texture.jpg
    )


set(QRC_FILES
    android/assets/fonts/fonts.qrc
    android/assets/shaders/shaders.qrc
)

source_group("main" FILES ${MAIN_FILES})
source_group("ui_controls" FILES ${UI_CONTROL_FILES})
source_group("assets" FILES ${ASSETS})
source_group("model_ui" FILES ${MODEL_FILES})
source_group("ui_pages" FILES ${UI_PAGES_FILES})
source_group("render" FILES ${OPENGL_FILES})
source_group("include" FILES ${INCLUDE_FILES})

set(PROJECT_SOURCES
        ${MAIN_FILES}
        ${UI_CONTROL_FILES}
        #${INCLUDE_FILES}
        #${OPENGL_FILES}
        #${UI_PAGES_FILES}
        #${MODEL_FILES}
)


include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(scara3D PRIVATE "C:/QtProject/scara3D/include/GLM")
#include_directories(scara3D PRIVATE "C:/QtProject/scara3D/include/STBImage")


################################编译前

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(scara3D
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${QRC_FILES}
        )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET scara3D APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(scara3D SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(scara3D
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(scara3D PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(scara3D PRIVATE Qt6::OpenGL)


set_property(TARGET scara3D APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.scara3D)
endif()
set_target_properties(scara3D PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS scara3D
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(scara3D)
endif()
