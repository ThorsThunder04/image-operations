
# libraries to include to every source file
set(libs "imgops;imgio")

# names of every source executable file
set(exec_sources
"rgb2y;sepYCrCb;bin_thresh_pgm;bin_thresh_ppm;tri_thresh_pgm;histogram_pgm;profile_pgm")


# I'm not too sure if this is an ideal practice, but it makes the most sense for me in my case here
foreach(srcfile IN ITEMS ${exec_sources})
    add_executable(${srcfile})
    target_sources(${srcfile} PRIVATE ${srcfile}.c)
    target_link_libraries(${srcfile} PRIVATE ${libs})
endforeach()

# add_executable(rgb2y)
# target_sources(rgb2y PRIVATE rgb2y.c)
# target_link_libraries(rgb2y PRIVATE ${libs})