# Final_Project_2560_Hash_Table_Auction
Hash Table Algorithm - Online auction system where users can bid for items in real-time, and the system must handle collisions and efficiently update bids.
Focus: Use Hash Tables to handle bidding collisions and update the current highest bids in constant time.

Online Setup:
1. Open the terminal in VS code
  
2. Create project Directory
  mkdir C:\projects\boost_test
  cd C:\projects\boost_test

3. Install Boost directory
   extract boost files to dir => C:\TestPro\boost_1_86_0
   navigate to directory

4. Run the following command:
   bootstrap.bat gcc
   add the path to the system where b2 file located
   .\b2 toolset=gcc --with-system variant=release link=static runtime-link=static
dir C:\TestPro\boost_1_86_0\stage\lib
C:\TestPro\boost_1_86_0\bin.v2\logs

5. Then run: b2 --with-system
6. Boost files are then created under: C:\TestPro\boost_1_86_0\stage\lib
7. Create a main file
   code main.cpp
8. Paste the following into main file:
   #include <boost/system/system_error.hpp> #include <iostream> int main() { try { throw boost::system::system_error(boost::system::errc::make_error_code(boost::system::errc::not_supported)); } catch (const boost::system::system_error& e) { std::cout << "Boost error: " << e.what() << std::endl; } return 0; }

9. Save the file with (Ctrl + S)
10. Add Boost to compiler Paths
    cl /std:c++17 /I C:\TestPro\boost_1_86_0 main.cpp /link /LIBPATH:C:\TestPro\boost_1_86_0\stage\lib boost_system.lib

11. Uploaded properties, tasks, and launch json to vs code
12. Compile with following command: (replace main.exe and main.cpp with explicit file name)
    g++ -std=c++17 -I"C:/TestPro/boost_1_86_0" -L"C:/TestPro/boost_install/lib" main.cpp -o main.exe "C:/TestPro/boost_install/lib/libboost_system-vc142-mt-x64-1_86.lib" "C:/TestPro/boost_install/lib/libboost_thread-vc142-mt-x64-1_86.lib" -lws2_32 -lmswsock
