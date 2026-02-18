
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

target_link_libraries(imgops PRIVATE imgio)
target_link_libraries(imgio PRIVATE imgops)