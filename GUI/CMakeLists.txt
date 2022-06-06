cmake_minimum_required(VERSION 3.5)

project(NTRU-GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ntru.h
        ntru.cpp
        C:/WinNTL-11_5_1/src/BasicThreadPool.cpp
        C:/WinNTL-11_5_1/src/FFT.cpp
        C:/WinNTL-11_5_1/src/FacVec.cpp
        C:/WinNTL-11_5_1/src/GF2.cpp
        C:/WinNTL-11_5_1/src/GF2E.cpp
        C:/WinNTL-11_5_1/src/GF2EX.cpp
        C:/WinNTL-11_5_1/src/GF2EXFactoring.cpp
        C:/WinNTL-11_5_1/src/GF2X.cpp
        C:/WinNTL-11_5_1/src/GF2X1.cpp
        C:/WinNTL-11_5_1/src/GF2XFactoring.cpp
        C:/WinNTL-11_5_1/src/GF2XVec.cpp
        C:/WinNTL-11_5_1/src/G_LLL_FP.cpp
        C:/WinNTL-11_5_1/src/G_LLL_QP.cpp
        C:/WinNTL-11_5_1/src/G_LLL_RR.cpp
        C:/WinNTL-11_5_1/src/G_LLL_XD.cpp
        C:/WinNTL-11_5_1/src/GetPID.cpp
        C:/WinNTL-11_5_1/src/GetTime.cpp
        C:/WinNTL-11_5_1/src/HNF.cpp
        C:/WinNTL-11_5_1/src/LLL.cpp
        C:/WinNTL-11_5_1/src/LLL_FP.cpp
        C:/WinNTL-11_5_1/src/LLL_QP.cpp
        C:/WinNTL-11_5_1/src/LLL_RR.cpp
        C:/WinNTL-11_5_1/src/LLL_XD.cpp
        C:/WinNTL-11_5_1/src/MatPrime.cpp
        C:/WinNTL-11_5_1/src/RR.cpp
        C:/WinNTL-11_5_1/src/WordVector.cpp
        C:/WinNTL-11_5_1/src/ZZ.cpp
        C:/WinNTL-11_5_1/src/ZZVec.cpp
        C:/WinNTL-11_5_1/src/ZZX.cpp
        C:/WinNTL-11_5_1/src/ZZX1.cpp
        C:/WinNTL-11_5_1/src/ZZXCharPoly.cpp
        C:/WinNTL-11_5_1/src/ZZXFactoring.cpp
        C:/WinNTL-11_5_1/src/ZZ_p.cpp
        C:/WinNTL-11_5_1/src/ZZ_pE.cpp
        C:/WinNTL-11_5_1/src/ZZ_pEX.cpp
        C:/WinNTL-11_5_1/src/ZZ_pEXFactoring.cpp
        C:/WinNTL-11_5_1/src/ZZ_pX.cpp
        C:/WinNTL-11_5_1/src/ZZ_pX1.cpp
        C:/WinNTL-11_5_1/src/ZZ_pXCharPoly.cpp
        C:/WinNTL-11_5_1/src/ZZ_pXFactoring.cpp
        C:/WinNTL-11_5_1/src/ctools.cpp
        C:/WinNTL-11_5_1/src/fileio.cpp
        C:/WinNTL-11_5_1/src/lip.cpp
        C:/WinNTL-11_5_1/src/lzz_p.cpp
        C:/WinNTL-11_5_1/src/lzz_pE.cpp
        C:/WinNTL-11_5_1/src/lzz_pEX.cpp
        C:/WinNTL-11_5_1/src/lzz_pEXFactoring.cpp
        C:/WinNTL-11_5_1/src/lzz_pX.cpp
        C:/WinNTL-11_5_1/src/lzz_pX1.cpp
        C:/WinNTL-11_5_1/src/lzz_pXCharPoly.cpp
        C:/WinNTL-11_5_1/src/lzz_pXFactoring.cpp
        C:/WinNTL-11_5_1/src/mat_GF2.cpp
        C:/WinNTL-11_5_1/src/mat_GF2E.cpp
        C:/WinNTL-11_5_1/src/mat_RR.cpp
        C:/WinNTL-11_5_1/src/mat_ZZ.cpp
        C:/WinNTL-11_5_1/src/mat_ZZ_p.cpp
        C:/WinNTL-11_5_1/src/mat_ZZ_pE.cpp
        C:/WinNTL-11_5_1/src/mat_lzz_p.cpp
        C:/WinNTL-11_5_1/src/mat_lzz_pE.cpp
        C:/WinNTL-11_5_1/src/mat_poly_ZZ.cpp
        C:/WinNTL-11_5_1/src/mat_poly_ZZ_p.cpp
        C:/WinNTL-11_5_1/src/mat_poly_lzz_p.cpp
        C:/WinNTL-11_5_1/src/pd_FFT.cpp
        C:/WinNTL-11_5_1/src/quad_float.cpp
        C:/WinNTL-11_5_1/src/quad_float1.cpp
        C:/WinNTL-11_5_1/src/thread.cpp
        C:/WinNTL-11_5_1/src/tools.cpp
        C:/WinNTL-11_5_1/src/vec_GF2.cpp
        C:/WinNTL-11_5_1/src/vec_GF2E.cpp
        C:/WinNTL-11_5_1/src/vec_RR.cpp
        C:/WinNTL-11_5_1/src/vec_ZZ.cpp
        C:/WinNTL-11_5_1/src/vec_ZZ_p.cpp
        C:/WinNTL-11_5_1/src/vec_ZZ_pE.cpp
        C:/WinNTL-11_5_1/src/vec_lzz_p.cpp
        C:/WinNTL-11_5_1/src/vec_lzz_pE.cpp
        C:/WinNTL-11_5_1/src/xdouble.cpp
)

include_directories(C:/WinNTL-11_5_1/include)
link_directories(C:/WinNTL-11_5_1/src/Release)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NTRU-GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NTRU-GUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NTRU-GUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NTRU-GUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(NTRU-GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(NTRU-GUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NTRU-GUI)
endif()
