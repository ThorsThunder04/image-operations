
add_library(imgio)

target_sources(imgio
    PRIVATE
        imgio.c
    PUBLIC
        FILE_SET imgio_headers
        TYPE HEADERS
        FILES
            imgio.h
)


add_library(imgops)

target_sources(imgops
    PRIVATE
        imgops.c
    PUBLIC
        FILE_SET imgops_headers
        TYPE HEADERS
        FILES
            imgops.h
)

target_compile_options(imgops PRIVATE -Wall -lm)
target_compile_options(imgio PRIVATE -Wall)

target_link_libraries(imgops PRIVATE imgio)