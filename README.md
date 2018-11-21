# replay
replay bchabc &amp; bchsv


# build

mkdir bin
cd bin
cmake ..
make


# usage
./broadcast url_abc url_sv auth_abc auth_sv txid

for example:
./broadcast 172.31.19.224:8332 127.0.0.1:8332 mining:war mining:war 802587ebc11aa758bd001c904c4ee4e0f1f84b5cf485f6d150cba0ae9d934f5b

the result are the hexs you need to broadcast in sv

