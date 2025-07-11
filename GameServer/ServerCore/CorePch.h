#pragma once



#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>

#include <optional>
using namespace std;
#include <boost/asio.hpp>


#include <assert.h>
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "Session.h"

#pragma comment(lib, "ws2_32.lib")