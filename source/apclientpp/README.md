# apclientpp (vendored)

Archipelago client library plus its header-only dependencies, copied in as plain files (no submodules).
Only headers and license files are kept; tests, docs and build scripts were dropped.

| Folder | Project | Version | Commit | License |
|---|---|---|---|---|
| `.` | [black-sliver/apclientpp](https://github.com/black-sliver/apclientpp) | 0.6.4 (master) | `79621690a3e845645f43888b0fe234a99c74892e` | MIT (`LICENSE`) |
| `wswrap/` | [black-sliver/wswrap](https://github.com/black-sliver/wswrap) | 1.03.00 (master) | `aeba7ac428028723fb26ce92488f260660f786b1` | MIT (`wswrap/LICENSE`) |
| `websocketpp/` | [zaphoyd/websocketpp](https://github.com/zaphoyd/websocketpp) | 0.8.2 | `56123c87598f8b1dd471be83ca841ceae07f95ba` | BSD-3 (`websocketpp/COPYING`) |
| `asio/` | [chriskohlhoff/asio](https://github.com/chriskohlhoff/asio) (standalone) | 1.30.2 | `12e0ce9e0500bf0f247dbd1ae894272656456079` | BSL-1.0 (`asio/LICENSE_1_0.txt`) |
| `json/` | [nlohmann/json](https://github.com/nlohmann/json) (single header) | 3.11.3 | release asset `json.hpp` | MIT (`json/LICENSE.MIT`) |

Do not bump asio past 1.31 without also updating websocketpp: websocketpp 0.8.2 still uses `io_service`,
which asio 1.33 removed.

## Prebuilt libraries

| Folder | Library | Version | License |
|---|---|---|---|
| `openssl/` | OpenSSL (libssl, libcrypto), for `wss://` | 3.6.4 | Apache-2.0 (`openssl/LICENSE.txt`) |
| `zlib/` | zlib, for permessage-deflate compression | 1.3.2 | zlib (`zlib/LICENSE.txt`) |

Built with vcpkg `openssl:x86-windows-static` and `zlib:x86-windows-static`, because the game is 32-bit and links
the static CRT. `lib/x86-MT` is the release build (`/MT`) and `lib/x86-MTd` is the debug build (`/MTd`).
vcpkg's debug `zlibd.lib` is renamed to `zlib.lib` so both configurations link the same name.

`cacert.pem` is curl's CA bundle (Mozilla data, 2026-08-13). The post-build step copies it next to the exe;
pass its path to the `APClient` constructor.

## Project settings

Include directories (relative to `source/`):

```
apclientpp
apclientpp/wswrap/include
apclientpp/websocketpp
apclientpp/asio/include
apclientpp/json/include
```

Preprocessor definitions: `ASIO_STANDALONE`, `_WIN32_WINNT=0x0601`, `AP_NO_SCHEMA`, `_WEBSOCKETPP_CPP11_STL_`.
`Common.h` already defines `WIN32_LEAN_AND_MEAN` before `windows.h`.

Compiler: `/std:c++17`, `/Zc:__cplusplus`.

Link: `ws2_32.lib`, `crypt32.lib`, plus the OpenSSL and zlib static libs.

Include `apclient.hpp` from a single `.cpp` so asio's headers stay out of the rest of the game.
Call `APClient::poll()` once per frame; all callbacks fire from inside `poll()` on the game thread.
