#-------------------------------------------------------------------------------
# GIMPACT
#-------------------------------------------------------------------------------
SET(extlibs_gimpact_gimpact_src
    GIMPACT/gim_boxpruning.h
    GIMPACT/gim_contact.h
    GIMPACT/gim_geometry.h
    GIMPACT/gim_math.h
    GIMPACT/gim_memory.h
    GIMPACT/gim_radixsort.h
    GIMPACT/gim_tri_capsule_collision.h
    GIMPACT/gim_tri_collision.h
    GIMPACT/gim_tri_sphere_collision.h
    GIMPACT/gim_trimesh.h
    GIMPACT/gimpact.h
)
SET(extlibs_gimpact_src
    gim_boxpruning.cc
    gim_contact.cc
    gim_math.cc
    gim_memory.cc
    gim_tri_tri_overlap.cc
    gim_trimesh.cc
    gim_trimesh_capsule_collision.cc
    gim_trimesh_ray_collision.cc
    gim_trimesh_sphere_collision.cc
    gim_trimesh_trimesh_collision.cc
    gimpact.cc
)
SET(extlibs_gimpact_SRCS
    ${extlibs_gimpact_src}
    ${extlibs_gimpact_gimpact_src}
)
SOURCE_GROUP(gimpact FILES ${extlibs_gimpact_src})
SOURCE_GROUP(gimpact\\gimpact FILES ${extlibs_gimpact_gimpact_src})

ADD_LIBRARY(gimpact STATIC ${extlibs_gimpact_SRCS})
N_SET_COMPILER_DEFAULTS()
ADD_DEPENDENCIES(gimpact foundation)
