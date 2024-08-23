cmake_minimum_required(VERSION 3.10)
project(SQLiteExample)

set(CMAKE_CXX_STANDARD 17)

# SQLite3のソースコードとヘッダーが含まれるディレクトリのパス
set(SQLITE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/sqlite-amalgamation-3460100")

# SQLite3のソースコードをコンパイルする
add_library(sqlite3 STATIC
        ${SQLITE_SOURCE_DIR}/sqlite3.c
)

# 実行可能ファイルを作成
add_executable(SQLiteExample main.cpp)

# SQLite3ライブラリをリンクする
target_link_libraries(SQLiteExample PRIVATE sqlite3)

# SQLite3のヘッダーをインクルードパスに追加
target_include_directories(SQLiteExample PRIVATE ${SQLITE_SOURCE_DIR})
