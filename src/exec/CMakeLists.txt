

add_executable(rgb2y)

target_sources(rgb2y
    PRIVATE
        rgb2y.c
)

target_link_libraries(rgb2y
    PRIVATE
        imgops
        imgio
)

add_executable(sepYCrCb)

target_sources(sepYCrCb PRIVATE sepYCrCb.c)
target_link_libraries(sepYCrCb PRIVATE imgops imgio)