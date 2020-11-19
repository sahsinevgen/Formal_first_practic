project(Libs)
set(SOURCES Determinization.cpp
            Make_one_letter_moves.cpp
            Minimization.cpp
            My_bitset.cpp
            Reg_expr_to_state_machine.cpp
            State_machine.cpp
            First_practic.cpp)
set(HEADERS Determinization.h
            Make_one_letter_moves.h
            Minimization.h
            My_bitset.h
            Reg_expr_to_state_machine.h
            State_machine.h
            First_practic.h)
add_library(libs ${SOURCES} ${HEADERS})
target_include_directories(libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})