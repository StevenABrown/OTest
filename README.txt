OTest is a Windows console mode program that demonstrates connectivity with Oanda trade server. 
OTest gets a single quote from https://api-fxpractice.oanda.com/v1/prices?instruments=EUR_USD, 
opens a trade at https://api-fxpractice.oanda.com/v1/accounts/<your_account_number>/orders,
displays open trades, closes the open trade, and then displays streaming quotes from 
https://stream-fxpractice.oanda.com/v1/prices?accountId=<your_account_number>&instruments=EUR_USD 
All output is to the screen in a Windows console window, and to output.txt.


Compiler and Project Settings

OTest.cpp runs in a Windows console window and is launched from the command prompt. It is designed to be 
compliled as a C++ Win32 console mode program in Microsoft Visual Studio Express 2013, which can be downloaded 
from Microsoft; registration to use that compiler is free. Necessary Project settings: no precompiled headers, 
add wininet.lib to the list of Additional Dependencies under Project Properties\Linker Input, and set the 
Character Set to single byte characters ("Not Set"). The compiler will issue warnings about insecurity of C 
library functions, but that is ok because we are being careful and using single-byte characters. To suppress 
the warnings, add _CRT_SECURE_NO_WARNINGS to the list of C/C++ Preprocessor Definitions. Replace all instances 
of <your_account_number> in the source code with your FxTrade Practice account number, and replace all instances 
of <your_access_token> with your FxTrade Practice access token, both without the corner brackets.
