#include "OAuth.h"

#include "Base64.h"
#include "sha1.h"
#include "URI.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>

using namespace std;

string
OAuth::createHeader() const {
  ostringstream s;
  s << "OAuth ";  
  if (!oauth_callback.empty()) s << "oauth_callback=\"" << URI::urlencode(oauth_callback) << "\", ";
  s << "oauth_consumer_key=\"" << URI::urlencode(oauth_consumer_key) << "\", ";
  s << "oauth_nonce=\"" << URI::urlencode(oauth_nonce) << "\", ";
  s << "oauth_signature=\"" << URI::urlencode(oauth_signature) << "\", ";
  s << "oauth_signature_method=\"HMAC-SHA1\", ";
  s << "oauth_timestamp=\"" << oauth_timestamp << "\", ";
  s << "oauth_token=\"" << URI::urlencode(oauth_token) << "\", ";
  s << "oauth_version=\"1.0\"";
  return s.str();
}

void
OAuth::initialize() {
  oauth_timestamp = time(0);
  oauth_nonce.clear();
  for (unsigned int i = 0; i < 42; i++) {
    int v = rand() % 62;
    if (v < 26) {
      oauth_nonce += char('A' + v);
    } else if (v < 52) {
      oauth_nonce += char('a' + v - 26);
    } else {
      oauth_nonce += char('0' + v - 52);
    }
  }

  // oauth_nonce = "f29dd9e9f37db7bf2a0267a49a78235b";
  // oauth_timestamp = 1358877129;

  map<string, string> data = content; // must be ordered
  if (!oauth_callback.empty()) data["oauth_callback"] = oauth_callback;
  data["oauth_consumer_key"] = oauth_consumer_key;
  data["oauth_nonce"] = oauth_nonce;
  data["oauth_signature_method"] = "HMAC-SHA1";
  data["oauth_timestamp"] = to_string(oauth_timestamp);
  data["oauth_token"] = oauth_token;
  data["oauth_version"] = "1.0";
  
  // data should be sorted alphabetically by the encoded key
  
  string parameters;
  for (auto it = data.begin(); it != data.end(); it++) {
    if (it != data.begin()) parameters += '&';
    parameters += it->first;
    parameters += '=';
    parameters += URI::urlencode(it->second);        
  }

  // cerr << "parameters = " << parameters << endl;

  string data2;
  data2 += http_method;
  data2 += '&';
  data2 += URI::urlencode(base_url);
  data2 += '&';
  data2 += URI::urlencode(parameters);

  cerr << "data2 = " << data2 << endl;
  
  string key;
  key += URI::urlencode(oauth_consumer_secret);
  key += '&';
  key += URI::urlencode(oauth_secret);

  // cerr << "key = " << key << endl;
  
  sha1_context ctx;
  sha1_hmac_starts( &ctx, (const unsigned char *)key.data(), key.size());
  sha1_hmac_update( &ctx, (const unsigned char *)data2.data(), data2.size() );
  unsigned char sha1sum[20];
  sha1_hmac_finish( &ctx, sha1sum );
  
  oauth_signature = Base64::encode(sha1sum, 20, 0, 0);

  // cerr << "signature = " << oauth_signature << endl;
}
