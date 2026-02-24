
# libraries to include to every source file
set(libs "imgops;imgio")

# names of every source executable file
set(exec_sources
"rgb2y;sepYCrCb;bin_thresh_pgm;bin_thresh_ppm;tri_thresh_pgm;histogram_pgm;profile_pgm;histogram_ppm;erode_bin_pgm;invert_pgm;dilate_bin_pgm;difference_pgm;filtre_flou1_pgm;filtre_flou2_pgm;filtre_flou1_ppm")


# I'm not too sure if this is an ideal practice, but it makes the most sense for me in my case here
foreach(srcfile IN ITEMS ${exec_sources})
    add_executable(${srcfile})
    target_sources(${srcfile} PRIVATE ${srcfile}.c)
    target_link_libraries(${srcfile} PRIVATE ${libs})
    target_compile_options(${srcfile} PRIVATE -Wall)
endforeach()
