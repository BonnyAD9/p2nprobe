file(GLOB src_
     "*.h"
     "*.cpp"
)

file(GLOB src_ CONFIGURE_DEPENDS "*.h" "*.cpp")

target_sources(p2nprobe PRIVATE ${src_})
