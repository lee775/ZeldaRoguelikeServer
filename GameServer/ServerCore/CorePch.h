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

#include <iostream>
using namespace std;

#include <winsock2.h>
#include<MSWSock.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <assert.h>
#include "SendBuffer.h"
#include "Session.h"

#include "SocketUtils.h"
#pragma comment(lib, "ws2_32.lib")