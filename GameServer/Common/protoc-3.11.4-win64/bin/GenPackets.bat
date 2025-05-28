pushd %~dp0

protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF ERRORLEVEL 1 PAUSE

XCOPY /Y Enum.pb.h "../../../GameServer"
XCOPY /Y Enum.pb.cc "../../../GameServer"
XCOPY /Y Struct.pb.h "../../../GameServer"
XCOPY /Y Struct.pb.cc "../../../GameServer"
XCOPY /Y Protocol.pb.h "../../../GameServer"
XCOPY /Y Protocol.pb.cc "../../../GameServer"

XCOPY /Y Enum.pb.h "../../../DummyClient"
XCOPY /Y Enum.pb.cc "../../../DummyClient"
XCOPY /Y Struct.pb.h "../../../DummyClient"
XCOPY /Y Struct.pb.cc "../../../DummyClient"
XCOPY /Y Protocol.pb.h "../../../DummyClient"
XCOPY /Y Protocol.pb.cc "../../../DummyClient"

XCOPY /Y Enum.pb.h "../../../GameCoding/GameCoding/GameCoding"
XCOPY /Y Enum.pb.cc "../../../GameCoding/GameCoding/GameCoding"
XCOPY /Y Struct.pb.h "../../../GameCoding/GameCoding/GameCoding"
XCOPY /Y Struct.pb.cc "../../../GameCoding/GameCoding/GameCoding"
XCOPY /Y Protocol.pb.h "../../../GameCoding/GameCoding/GameCoding"
XCOPY /Y Protocol.pb.cc "../../../GameCoding/GameCoding/GameCoding"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE