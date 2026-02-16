
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