1. Create new project in Visual Studio (not VS Code, I use Insiders but any version of VS should work).
   Create the files in the project to copy the program files from this repository (main.cpp, Database.h, and Database.cpp) and proceed in copying those programs into the project files.

2. Install a C++ Connector, I'm using 26.10. Install it here https://dev.mysql.com/downloads/connector/cpp/

3. Configure VS project to include the mySQL files:
     a) Switch "Debug" to "Release" in the top bar, then click on "Project" at the very top and click on the very bottom option "[ProjectName] properties". (Screenshot 3.a in docs)
     b) Set the "Configuration" from "Debug" to "Release" at the top, and set "Platform" to "x64" also at the top. (Screenshot 3.b in docs)
     c) Once you are in the "Property Pages" pop-up click on "C/C++" and then under it "General" and edit the "Additional Include Directories" in the right box to be the path into your jdbc folder,
        (in my case it was C:\Program Files\MySQL\MySQL Connector C++ 26.10\include\jdbc) (Screenshot 3.c in docs)
     d) Then under the same "C/C++" property, select "Preprocessor" and then "Preprocessor Definitions" in the right box. Then just add "STATIC_CONCPP;" to the beginning of the text. (Screenshot 3.d in docs)
     e) Under the same "C/C++" property, select "Code Generation" and make sure that "Runtime Library" in the right box is set to "Multi-threaded DLL (/MD)", it is normally set to this by default. (Screenshot 3.e in docs)
     f) Then select "Linker" property, and select under it "General" and change the "Additional Library Directories" to the path to your vs14 folder in the connector download folder
         (in my case it was C:\Program Files\MySQL\MySQL Connector C++ 26.10\lib64\vs14). (Screenshot 3.f in docs folder)
     g) Then under the same "Linker" property, select "Input" and edit the "Additional Dependencies" to be "mysqlcppconn-static.lib". (Screenshot 3.g in docs folder)
     h) After that we can press "OK" and exit the pop-up and your VS should be configured for MySQL Library support.

4. We need to add two files from the Connector to the project folder next to the .exe application file that runs the program. Find the "libssl-3-x64.dll" and "libcrypto-3-x64.dll" in the "lib64" folder in your connector and copy them.
   Then paste them into the folder that holds you .exe file for your program in VS, this should be in "/x64/Release/" path in your project solution folder. (Screenshots 4.a and 4.b in docs)

5. Now you can finally run the program.
