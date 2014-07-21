// OTest.cpp 1.42, a test program to GET a single quote from 
// https://api-fxpractice.oanda.com/v1/prices?instruments=EUR_USD, to open a trade at 
// https://api-fxpractice.oanda.com/v1/accounts/<your_account_number>/orders,
// to display open trades, close the oldest open trade, and then to display streaming quotes from
// https://stream-fxpractice.oanda.com/v1/prices?accountId=<your_account_number>&instruments=EUR_USD 

// All output is to the screen in a Windows console window, and to output.txt.
// Open Source software by Steve Brown, mrDOTstevenDOTbrownATverizonDOTnet
// Compiler information and project settings are in comment at end of this file.

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "stdafx.h"
#include "Wininet.h"
using namespace std;

// Global variables are named with underscore after type specifier.
HINTERNET h_Inet;
const char* str_Array[] = {"application/x-www-form-urlencoded", "application/json", "text/*", NULL};
char ch_Buffer[4096], ch_Line[256];
ofstream of_OutFile;

int main()
{ BOOL bResult; char *chPtr0, *chPtr1, *chPtr2; DWORD dw1, dw2, dwIndex;
  HINTERNET hInet, hRequest; int i, iChunks;

  of_OutFile.open ("output.txt");  // Output will be written to the screen and to output.txt.

  h_Inet = InternetOpen("OTest",                    // _In_ LPCTSTR lpszAgent
                        INTERNET_OPEN_TYPE_DIRECT,  // _In_ DWORD dwAccessType
                        NULL,                       // _In_ LPCTSTR lpszProxyName
                        NULL,                       // _In_ LPCTSTR lpszProxyBypass
                        NULL);                      // _In_ DWORD dwFlags

  if(h_Inet == NULL) 
  { cout       << "\nCall to InternetOpen returned NULL. Aborting.\n"; 
    of_OutFile << "\nCall to InternetOpen returned NULL. Aborting.\n";
    of_OutFile.close();
    return 0;
  }
  else
  { cout       << "\nCall to InternetOpen returned valid handle.\n";
    of_OutFile << "\nCall to InternetOpen returned valid handle.\n";
  }

  hInet = InternetConnect(h_Inet,        // _In_ HINTERNET hInternet
            "api-fxpractice.oanda.com",  // _In_ LPCTSTR lpszServerName
            INTERNET_DEFAULT_HTTPS_PORT, // _In_ INTERNET_PORT nServerPort
            NULL, // _In_ LPCTSTR lpszUsername  Don't send your user name if you don't have to. 
            NULL, // _In_ LPCTSTR lpszPassword  Don't send your password if you don't have to.
            INTERNET_SERVICE_HTTP,       // _In_ DWORD dwService
            NULL,                        // _In_ DWORD dwFlags
            NULL);                       // _In_ DWORD dwContext

  if(hInet == NULL) 
  { cout       << "Call to InternetConnect returned NULL.\n";
    of_OutFile << "Call to InternetConnect returned NULL.\n";
  }
  else
  { cout       << "Call to InternetConnect returned valid handle.\n";
    of_OutFile << "Call to InternetConnect returned valid handle.\n";
  }

// Set up request to get a single quote.
  hRequest = HttpOpenRequest(hInet,               // _In_ HINTERNET hConnect
               "GET",                             // _In_ LPCTSTR lpszVerb
               "/v1/prices?instruments=EUR_USD",  // _In_ LPCTSTR lpszObjectName, target on server
               "HTTP/1.1",                        // _In_ LPCTSTR lpszVersion
               NULL,                              // _In_ LPCTSTR lpszReferer
               str_Array,                         // _In_ LPCTSTR *lplpszAcceptTypes
               INTERNET_FLAG_SECURE,              // _In_ DWORD dwFlags
               NULL);                             // _In_ DWORD dwContext

  if(hRequest == NULL) 
  { cout       << "Call to HttpOpenRequest returned NULL.\n";
    of_OutFile << "Call to HttpOpenRequest returned NULL.\n";
  }
  else
  { cout       << "Call to HttpOpenRequest returned valid handle.\n";
    of_OutFile << "Call to HttpOpenRequest returned valid handle.\n";
  }

  bResult = HttpSendRequest(hRequest,  // _In_ HINTERNET hRequest
              "Authorization: Bearer <your_access_token>", //_In_ LPCTSTR lpszHeaders
                            -1,        // _In_ DWORD dwHeadersLength
                            NULL,      // _In_ LPVOID lpOptional
                            0);        // _In_ DWORD dwOptionalLength
  
  if(bResult)
  { cout       << "Call to HttpSendRequest returned TRUE.\n\n";
    of_OutFile << "Call to HttpSendRequest returned TRUE.\n\n";
  }
  else
  { dw1 = GetLastError();
    cout       << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
    of_OutFile << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                     // _In_ HINTERNET hRequest
                          HTTP_QUERY_RAW_HEADERS_CRLF,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                    // _Inout_ LPVOID lpvBuffer
                          &dw1,                         // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                    // _Inout_ LPDWORD lpdwIndex

  if (bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    cout << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                   // _In_ HINTERNET hRequest
                          HTTP_QUERY_CONTENT_LENGTH,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                  // _Inout_ LPVOID lpvBuffer
                          &dw1,                       // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                  // _Inout_ LPDWORD lpdwIndex

  ch_Buffer[dw1] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    cout << "String in ch_Buffer: " << ch_Buffer << "\n";
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer: " << ch_Buffer << "\n";
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = atol(ch_Buffer);  // Convert CONTENT_LENGTH string in ch_Buffer to numerical value.

  bResult = InternetReadFile(hRequest,  // _In_  HINTERNET hFile
                             ch_Buffer, // _Out_ LPVOID lpBuffer
                             dw1,       // _In_  DWORD dwNumberOfBytesToRead
                             &dw2);     // _Out_ LPDWORD lpdwNumberOfBytesRead

  ch_Buffer[dw2] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "\nCall to InternetReadFile returned TRUE.\n";
    cout << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    cout << "CONTENT string written to ch_Buffer (starting on next line):\n";
    cout << ch_Buffer << endl;
    of_OutFile << "\nCall to InternetReadFile returned TRUE.\n";
    of_OutFile << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    of_OutFile << "CONTENT string written to ch_Buffer (starting on next line):\n";
    of_OutFile << ch_Buffer << endl;
  }
  else
  { cout       << "\nCall to InternetReadFile returned FALSE. ";
    of_OutFile << "\nCall to InternetReadFile returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << endl;
    of_OutFile << "Error code " << dw2 << endl;
  }

// Release request handle before making new request.
  InternetCloseHandle(hRequest);

// Set up request to BUY 100 units of EUR/USD.
  hRequest = HttpOpenRequest(hInet,                         // _In_ HINTERNET hConnect
                             "POST",                        // _In_ LPCTSTR lpszVerb
                             "/v1/accounts/<your_account_number>/orders", // _In_ LPCTSTR lpszObjectName
                             "HTTP/1.1",                    // _In_ LPCTSTR lpszVersion
                             NULL,                          // _In_ LPCTSTR lpszReferer
                             str_Array,                     // _In_ LPCTSTR *lplpszAcceptTypes
                             INTERNET_FLAG_SECURE,          // _In_ DWORD dwFlags
                             NULL);                         // _In_ DWORD dwContext

  if(hRequest == NULL) 
  { cout       << "\nCall to HttpOpenRequest returned NULL.\n";
    of_OutFile << "\nCall to HttpOpenRequest returned NULL.\n";
  }
  else
  { cout       << "\nCall to HttpOpenRequest returned valid handle.\n";
    of_OutFile << "\nCall to HttpOpenRequest returned valid handle.\n";
  }

  HttpAddRequestHeaders(hRequest,
    "Authorization: Bearer <your_access_token>" , // _In_  LPCTSTR lpszHeaders,
                        -1,  //_In_  DWORD dwHeadersLength
                        0);  //_In_  DWORD dwModifiers

// Specify parameters of order in body of request.
  char *chValues = "instrument=EUR_USD&units=100&side=buy&type=market";

  bResult = HttpSendRequest(hRequest,                                           // _In_ HINTERNET hRequest
                            "content-type: application/x-www-form-urlencoded",  // _In_ LPCTSTR lpszHeaders
                            -1,                                                 // _In_ DWORD dwHeadersLength
                            chValues,                                           // _In_ LPVOID lpOptional
                            strlen(chValues));                                  // _In_ DWORD dwOptionalLength
  
  if(bResult)
  { cout       << "Call to HttpSendRequest returned TRUE.\n";
    of_OutFile << "Call to HttpSendRequest returned TRUE.\n";
  }
  else
  { dw1 = GetLastError();
    cout       << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
    of_OutFile << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,         // _In_ HINTERNET hRequest
              HTTP_QUERY_RAW_HEADERS_CRLF,  // _In_ DWORD dwInfoLevel
              ch_Buffer,                    // _Inout_ LPVOID lpvBuffer
              &dw1,                         // _Inout_ LPDWORD lpdwBufferLength
              &dwIndex);                    // _Inout_ LPDWORD lpdwIndex

  if (bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    cout << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                   // _In_ HINTERNET hRequest
                          HTTP_QUERY_CONTENT_LENGTH,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                  // _Inout_ LPVOID lpvBuffer
                          &dw1,                       // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                  // _Inout_ LPDWORD lpdwIndex

  ch_Buffer[dw1] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    cout << "String in ch_Buffer: " << ch_Buffer << "\n";
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer: " << ch_Buffer << "\n";
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = atol(ch_Buffer); // Convert CONTENT_LENGTH string in ch_Buffer to numerical value.

  bResult = InternetReadFile(hRequest,  // _In_  HINTERNET hFile
                             ch_Buffer, // _Out_ LPVOID lpBuffer
                             dw1,       // _In_  DWORD dwNumberOfBytesToRead
                             &dw2);     // _Out_ LPDWORD lpdwNumberOfBytesRead

  ch_Buffer[dw2] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "\nCall to InternetReadFile returned TRUE.\n";
    cout << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    cout << "CONTENT string written to ch_Buffer (starting on next line):\n";
    cout << ch_Buffer << endl;
    of_OutFile << "\nCall to InternetReadFile returned TRUE.\n";
    of_OutFile << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    of_OutFile << "CONTENT string written to ch_Buffer (starting on next line):\n";
    of_OutFile << ch_Buffer << endl;
  }
  else
  { cout       << "\nCall to InternetReadFile returned FALSE. ";
    of_OutFile << "\nCall to InternetReadFile returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << endl;
    of_OutFile << "Error code " << dw2 << endl;
  }

// Close handle of previous request before initializing hRequest for new request to list trades.
  InternetCloseHandle(hRequest);

// Set "count" parameter to maximum number of trades to list.
  hRequest = HttpOpenRequest(hInet,                                 // _In_ HINTERNET hConnect
                             "GET",                                 // _In_ LPCTSTR lpszVerb
                             "/v1/accounts/<your_account_number>/trades?count=15", // _In_ LPCTSTR lpszObjectName
//                           "/v1/accounts/<your_account_number>",  // Use this line to get account information.
                             "HTTP/1.1",                            // _In_ LPCTSTR lpszVersion
                             NULL,                                  // _In_ LPCTSTR lpszReferer
                             str_Array,                             // _In_ LPCTSTR *lplpszAcceptTypes
                             INTERNET_FLAG_SECURE,                  // _In_ DWORD dwFlags
                             NULL);                                 // _In_ DWORD dwContext

  if(hRequest == NULL) 
  { cout       << "Call to HttpOpenRequest returned NULL.\n";
    of_OutFile << "Call to HttpOpenRequest returned NULL.\n";
  }
  else
  { cout       << "\nCall to HttpOpenRequest returned valid handle.\n";
    of_OutFile << "\nCall to HttpOpenRequest returned valid handle.\n";
  }

  bResult = HttpSendRequest(hRequest,  // _In_ HINTERNET hRequest
              "Authorization: Bearer <your_access_token>", // _In_ LPCTSTR lpszHeaders
                            -1,        // _In_ DWORD dwHeadersLength
                            NULL,      // _In_ LPVOID lpOptional
                            0);        // _In_ DWORD dwOptionalLength
  
  if(bResult)
  { cout       << "Call to HttpSendRequest returned TRUE.\n";
    of_OutFile << "Call to HttpSendRequest returned TRUE.\n";
  }
  else
  { dw1 = GetLastError();
    cout       << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
    of_OutFile << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                     // _In_ HINTERNET hRequest
                          HTTP_QUERY_RAW_HEADERS_CRLF,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                    // _Inout_ LPVOID lpvBuffer
                          &dw1,                         // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                    // _Inout_ LPDWORD lpdwIndex

  if (bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    cout << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                   // _In_ HINTERNET hRequest
                          HTTP_QUERY_CONTENT_LENGTH,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                  // _Inout_ LPVOID lpvBuffer
                          &dw1,                       // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                  // _Inout_ LPDWORD lpdwIndex

  ch_Buffer[dw1] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    cout << "String in ch_Buffer: " << ch_Buffer << "\n";
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer: " << ch_Buffer << "\n";
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = atol(ch_Buffer); // Convert CONTENT_LENGTH string in ch_Buffer to numerical value.

  bResult = InternetReadFile(hRequest,  // _In_  HINTERNET hFile
                             ch_Buffer, // _Out_ LPVOID lpBuffer
                             dw1,       // _In_  DWORD dwNumberOfBytesToRead
                             &dw2);     // _Out_ LPDWORD lpdwNumberOfBytesRead

  ch_Buffer[dw2] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "\nCall to InternetReadFile returned TRUE.\n";
    cout << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    cout << "CONTENT string written to ch_Buffer (starting on next line):\n";
    cout << ch_Buffer << endl;
    of_OutFile << "\nCall to InternetReadFile returned TRUE.\n";
    of_OutFile << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    of_OutFile << "CONTENT string written to ch_Buffer (starting on next line):\n";
    of_OutFile << ch_Buffer << endl;
  }
  else
  { cout       << "\nCall to InternetReadFile returned FALSE. ";
    of_OutFile << "\nCall to InternetReadFile returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << endl;
    of_OutFile << "Error code " << dw2 << endl;
  }

// The oldest trade must be closed first, so get the trade_id of the last trade in JSON string in ch_Buffer, so 
// it can be closed. We will use functions from the C library to do that, but JSON parsing libraries are available.

  strcpy(ch_Line, "/v1/accounts/<your_account_number>/trades/");  // trade_id will be appended to this string.

// Find last occurrance of "id" in JSON string.
  chPtr0 = chPtr1 = ch_Buffer;
  while(chPtr0 != NULL)
  { chPtr1 = chPtr0;
    chPtr0 = strstr(chPtr1+1, "\"id\"");
  }

  chPtr1 += 7; // Advance pointer to start of trade_id number.
// Find comma at end of number, and write NULL to that address.
  chPtr2 = strchr(chPtr1, 44);  chPtr2[0] = 0;
// Append trade_id number to line buffer.
  strcat(ch_Line, chPtr1);
  cout       << "Last trade_id found: " << chPtr1 << "\n" << endl;
  of_OutFile << "Last trade_id found: " << chPtr1 << "\n" << endl;

// Release request handle before making new request.
  InternetCloseHandle(hRequest);

// Close the trade.
  hRequest = HttpOpenRequest(hInet,                 // _In_ HINTERNET hConnect
                             "DELETE",              // _In_ LPCTSTR lpszVerb
                             ch_Line,               // _In_ LPCTSTR lpszObjectName
                             "HTTP/1.1",            // _In_ LPCTSTR lpszVersion
                             NULL,                  // _In_ LPCTSTR lpszReferer
                             str_Array,             // _In_ LPCTSTR *lplpszAcceptTypes
                             INTERNET_FLAG_SECURE,  // _In_ DWORD dwFlags
                             NULL);                 // _In_ DWORD dwContext

  if(hRequest == NULL) 
  { cout       << "Call to HttpOpenRequest returned NULL.\n";
    of_OutFile << "Call to HttpOpenRequest returned NULL.\n";
  }
  else
  { cout       << "Call to HttpOpenRequest returned valid handle.\n";
    of_OutFile << "Call to HttpOpenRequest returned valid handle.\n";
  }

  bResult = HttpSendRequest(hRequest,  // _In_ HINTERNET hRequest
              "Authorization: Bearer <your_access_token>", // _In_ LPCTSTR lpszHeaders
                            -1,        // _In_ DWORD dwHeadersLength
                            NULL,      // _In_ LPVOID lpOptional
                            0);        // _In_ DWORD dwOptionalLength
  
  if(bResult)
  { cout       << "Call to HttpSendRequest returned TRUE.\n";
    of_OutFile << "Call to HttpSendRequest returned TRUE.\n";
  }
  else
  { dw1 = GetLastError();
    cout       << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
    of_OutFile << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                     // _In_ HINTERNET hRequest
                          HTTP_QUERY_RAW_HEADERS_CRLF,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                    // _Inout_ LPVOID lpvBuffer
                          &dw1,                         // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                    // _Inout_ LPDWORD lpdwIndex

  if (bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    cout << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                   // _In_ HINTERNET hRequest
                          HTTP_QUERY_CONTENT_LENGTH,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                  // _Inout_ LPVOID lpvBuffer
                          &dw1,                       // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                  // _Inout_ LPDWORD lpdwIndex

  ch_Buffer[dw1] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    cout << "String in ch_Buffer: " << ch_Buffer << "\n";
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for CONTENT LENGTH: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer: " << ch_Buffer << "\n";
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

  dw1 = atol(ch_Buffer); // Convert CONTENT_LENGTH string in ch_Buffer to numerical value.

  bResult = InternetReadFile(hRequest,  // _In_  HINTERNET hFile
                             ch_Buffer, // _Out_ LPVOID lpBuffer
                             dw1,       // _In_  DWORD dwNumberOfBytesToRead
                             &dw2);     // _Out_ LPDWORD lpdwNumberOfBytesRead

  ch_Buffer[dw2] = 0;  // Terminate string in ch_Buffer by appending a null character.

  if(bResult)
  { cout << "\nCall to InternetReadFile returned TRUE.\n";
    cout << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    cout << "CONTENT string written to ch_Buffer (starting on next line):\n";
    cout << ch_Buffer << "\n" << endl;
    of_OutFile << "\nCall to InternetReadFile returned TRUE.\n";
    of_OutFile << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
    of_OutFile << "CONTENT string written to ch_Buffer (starting on next line):\n";
    of_OutFile << ch_Buffer << "\n" << endl;
  }
  else
  { cout       << "\nCall to InternetReadFile returned FALSE. ";
    of_OutFile << "\nCall to InternetReadFile returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << endl;
    of_OutFile << "Error code " << dw2 << endl;
  }

// The next section displays streaming quotes, calling InternetReadFile() in synchronous mode.

// Close handles before initializing to new values to connect to streaming server.
  InternetCloseHandle(hRequest);
  InternetCloseHandle(hInet);

  hInet = InternetConnect(h_Inet,          // _In_ HINTERNET hInternet
            "stream-fxpractice.oanda.com", // _In_ LPCTSTR lpszServerName
            INTERNET_DEFAULT_HTTPS_PORT,   // _In_ INTERNET_PORT nServerPort
            NULL, // _In_ LPCTSTR lpszUsername, Don't send your user name if you don't have to.
            NULL, // _In_ LPCTSTR lpszPassword, Dont' send your password if you don't have to.
            INTERNET_SERVICE_HTTP,         // _In_ DWORD dwService
            NULL,                          // _In_ DWORD dwFlags
            NULL);                         // _In_ DWORD dwContext

  if(hInet == NULL) 
  { cout       << "Call to InternetConnect returned NULL.\n";
    of_OutFile << "Call to InternetConnect returned NULL.\n";
  }
  else
  { cout       << "Call to InternetConnect returned valid handle.\n";
    of_OutFile << "Call to InternetConnect returned valid handle.\n";
  }

// Set up request to get streaming quotes.
  hRequest = HttpOpenRequest(hInet,  // _In_ HINTERNET hConnect
               "GET",                // _In_ LPCTSTR lpszVerb
               "/v1/prices?accountId=<your_account_number>&instruments=EUR_USD", // _In_ LPCTSTR lpszObjectName
               "HTTP/1.1",           // _In_ LPCTSTR lpszVersion
               NULL,                 // _In_ LPCTSTR lpszReferer
               str_Array,            // _In_ LPCTSTR *lplpszAcceptTypes
               INTERNET_FLAG_SECURE | INTERNET_FLAG_KEEP_CONNECTION, // _In_ DWORD dwFlags
               NULL);                // _In_ DWORD dwContext

  if(hRequest == NULL) 
  { cout       << "Call to HttpOpenRequest returned NULL.\n";
    of_OutFile << "Call to HttpOpenRequest returned NULL.\n";
  }
  else
  { cout       << "Call to HttpOpenRequest returned valid handle.\n";
    of_OutFile << "Call to HttpOpenRequest returned valid handle.\n";
  }

  bResult = HttpSendRequest(hRequest,  // _In_ HINTERNET hRequest
              "Authorization: Bearer <your_access_token>",  // _In_ LPCTSTR lpszHeaders
                            -1,        // _In_ DWORD dwHeadersLength
                            NULL,      // _In_ LPVOID lpOptional
                            0);        // _In_ DWORD dwOptionalLength

  if(bResult)
  { cout       << "Call to HttpSendRequest returned TRUE.\n";
    of_OutFile << "Call to HttpSendRequest returned TRUE.\n";
  }
  else
  { dw1 = GetLastError();
    cout       << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
    of_OutFile << "Call to HttpSendRequest returned FALSE. Error code " << dw1 << "\n"; 
  }

  dw1 = 4096;  // size of ch_Buffer
  dwIndex = 0;

  bResult = HttpQueryInfo(hRequest,                     // _In_ HINTERNET hRequest
                          HTTP_QUERY_RAW_HEADERS_CRLF,  // _In_ DWORD dwInfoLevel
                          ch_Buffer,                    // _Inout_ LPVOID lpvBuffer
                          &dw1,                         // _Inout_ LPDWORD lpdwBufferLength
                          &dwIndex);                    // _Inout_ LPDWORD lpdwIndex

  if (bResult)
  { cout << "Call to HttpQueryInfo returned TRUE.\n";
    cout << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    cout << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
    of_OutFile << "Call to HttpQueryInfo returned TRUE.\n";
    of_OutFile << "Number of bytes written to ch_Buffer for HTTP_QUERY_RAW_HEADERS_CRLF: " << dw1 << "\n";
    of_OutFile << "String in ch_Buffer (starting on next line):\n" << ch_Buffer;
  }
  else
  { cout       << "Call to HttpQueryInfo returned FALSE. ";
    of_OutFile << "Call to HttpQueryInfo returned FALSE. ";
    dw2 = GetLastError();
    cout       << "Error code " << dw2 << "\n";
    of_OutFile << "Error code " << dw2 << "\n";
  }

// Set iChunks equal to the number of streaming chunks to receive, including ticks and heartbeats.
  iChunks = 4;

  for(i = 0; i < iChunks; i++)
  { dw1 = 4096;  // size of ch_Buffer
    bResult = InternetReadFile(hRequest,  // _In_  HINTERNET hFile
                               ch_Buffer, // _Out_ LPVOID lpBuffer
                               dw1,       // _In_  DWORD dwNumberOfBytesToRead
                               &dw2);     // _Out_ LPDWORD lpdwNumberOfBytesRead

    ch_Buffer[dw2] = 0;  // Terminate string in ch_Buffer by appending a null character.

    if(bResult)
    { cout       << "Call to InternetReadFile returned TRUE.\n";
      of_OutFile << "Call to InternetReadFile returned TRUE.\n";
      cout       << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
      of_OutFile << "Size of CONTENT string written to ch_Buffer: " << dw2 << "\n";
      cout       << "CONTENT string written to ch_Buffer (starting on next line):\n";
      of_OutFile << "CONTENT string written to ch_Buffer (starting on next line):\n";
      cout       << ch_Buffer << endl;
      of_OutFile << ch_Buffer << endl;
    }
    else
    { dw2 = GetLastError();
      cout       << "\nCall to InternetReadFile returned FALSE. Error code " << dw2 << endl;
      of_OutFile << "\nCall to InternetReadFile returned FALSE. Error code " << dw2 << endl;
      if(dw2 == ERROR_INTERNET_TIMEOUT) break; // error code 12002
    }
  }//END for(i = 0; i < iChunks; i++)

//Close open handles.
  InternetCloseHandle(hRequest);
  InternetCloseHandle(hInet);
  InternetCloseHandle(h_Inet);
//Close output.txt.
  of_OutFile.close();
  return 0;
} //END of main() 


/*/////////////////////////////////////////////////////////////////////////////////////////////////////

Compiler and Project Settings

OTest.cpp runs in a Windows console window and is launched from the command prompt. It is designed 
to be compliled as a C++ Win32 console mode program in Microsoft Visual Studio Express 2013, which 
can be downloaded from Microsoft; registration to use that compiler is free. Necessary Project settings: 
no precompiled headers, add wininet.lib to the list of Additional Dependencies under Project Properties\
Linker Input, and set the Character Set to single byte characters ("Not Set"). The compiler will issue
warnings about insecurity of C library functions, but that is ok because we are being careful and using 
single-byte characters. To suppress the warnings, add _CRT_SECURE_NO_WARNINGS to the list of C/C++ 
Preprocessor Definitions. Replace all instances of <your_account_number> in the source code with your 
FxTrade Practice account number, and replace all instances of <your_access_token> with your FxTrade 
Practice access token, both without the corner brackets.

*//////////////////////////////////////////////////////////////////////////////////////////////////////

