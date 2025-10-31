if (NOT EXISTS "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/install_manifest.txt\"")
endif(NOT EXISTS "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/install_manifest.txt")

file(READ "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach (file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    execute_process(
        COMMAND /usr/bin/cmake -E remove "$ENV{DESTDIR}${file}"
        OUTPUT_VARIABLE rm_out
        RESULT_VARIABLE rm_retval
    )
    if(NOT ${rm_retval} EQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif (NOT ${rm_retval} EQUAL 0)
endforeach(file)

