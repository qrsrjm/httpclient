#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_

#include <string>

class HTTPRequest {
 public:
  enum RequestType { 
    GET = 1,
    POST,
    PUT,
    DEL
  };

 HTTPRequest(RequestType _type, const std::string & _uri) : type(_type), uri(_uri) { }

  const RequestType getType() const { return type; }
  const std::string & getURI() const { return uri; }
  const std::string & getContent() const { return content; }
  const std::string & getContentType() const { return content_type; }
  bool getFollowLocation() const { return follow_location; }
  int getTimeout() const { return timeout; }
  
  void setContent(const std::string & _content) { content = _content; }
  void setContentType(const std::string & _content_type) { content_type = _content_type; }

  void setFollowLocation(bool f) { follow_location = f; }
  void setTimeout(int t) { timeout = t; }
  
 private:
  RequestType type;
  std::string uri;
  std::string content;
  std::string content_type;
  bool follow_location = true;
  int timeout = 0;
};

#endif
