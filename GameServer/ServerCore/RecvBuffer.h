#pragma once

/*--------------
	RecvBuffer
----------------*/

class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	int32			DataSize() { return _writePos - _readPos; }
	int32			FreeSize() { return _capacity - _writePos; }

private:
	// 실제 버퍼의 크기
	int32			_capacity = 0;
	// 유저가 설정한 버퍼의 크기
	int32			_bufferSize = 0;
	int32			_readPos = 0;
	int32			_writePos = 0;
	vector<BYTE>	_buffer;
};

