// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_PROTORPC_RPCCODEC_H
#define MUDUO_NET_PROTORPC_RPCCODEC_H

#include <muduo/base/Timestamp.h>
#include <muduo/net/protobuf/ProtobufCodecLite.h>

#include <boost/noncopyable.hpp>

namespace muduo
{
namespace net
{

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class RpcMessage;

// wire format
//
// Field     Length  Content
//
// size      4-byte  N+8
// "RPC0"    4-byte
// payload   N-byte
// checksum  4-byte  adler32 of "RPC0"+payload
//

// TODO: re-implement with typedef of ProtobufCodecT
class RpcCodec : boost::noncopyable
{
 public:
  typedef std::function<void (const TcpConnectionPtr&,
                              const RpcMessage&,
                              Timestamp)> ProtobufMessageCallback;

  typedef ProtobufCodecLite::ErrorCallback ErrorCallback;

  explicit RpcCodec(const ProtobufMessageCallback& messageCb,
                    const ErrorCallback& errorCb = ProtobufCodecLite::defaultErrorCallback);

  void send(const TcpConnectionPtr& conn,
            const RpcMessage& message);

  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf,
                 Timestamp receiveTime);

  // internal
  void onRpcMessage(const TcpConnectionPtr&,
                    const MessagePtr&,
                    Timestamp);
  void fillEmptyBuffer(muduo::net::Buffer* buf, const RpcMessage& message);

 private:
  ProtobufMessageCallback messageCallback_;
  ProtobufCodecLite codec_;
};

}
}

#endif  // MUDUO_NET_PROTORPC_RPCCODEC_H
