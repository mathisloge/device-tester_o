cmake_minimum_required (VERSION 3.5)
project (device_tester_docs NONE)

find_program (ASCIIDOCTOR_EXE asciidoctor)
find_program (ASCIIDOCTOR_PDF_EXE asciidoctor-pdf)

if(NOT ASCIIDOCTOR_EXE)
    message(WARNING "asciidoctor not found. Could not create targets for docs")
endif()
if(NOT ASCIIDOCTOR_PDF_EXE)
    message(WARNING "asciidoctor-pdf not found. Could not create targets for docs")
endif()

set (MAIN_FILE "${PROJECT_SOURCE_DIR}/main.adoc")
add_custom_target("pdf" ALL
    COMMAND ${ASCIIDOCTOR_PDF_EXE} ${MAIN_FILE}
    COMMAND ${CMAKE_COMMAND} -E rename "${PROJECT_SOURCE_DIR}/main.pdf" "${CMAKE_CURRENT_BINARY_DIR}/device_tester.pdf"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target("site" ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/site/images
    COMMAND ${ASCIIDOCTOR_EXE} -D ${CMAKE_CURRENT_BINARY_DIR}/site -o index.html ${MAIN_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/images ${CMAKE_CURRENT_BINARY_DIR}/site/images/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/bib ${CMAKE_CURRENT_BINARY_DIR}/site/bib/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
