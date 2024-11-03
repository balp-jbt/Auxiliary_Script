../bin/rsa -m key-gen -l 1024 --pub-path "./data/id_rsa.pub" --priv-path "./data/id_rsa"
wait
../bin/rsa -m encrypt --pub-path "./data/id_rsa.pub" -i ./plain-text -o ./data/cipher-text
wait
../bin/rsa -m decrypt --pub-path "./data/id_rsa.pub" --priv-path "./data/id_rsa" -i ./data/cipher-text -o ./data/decrypt-text
wait
../bin/rsa -m sign --pub-path "./data/id_rsa.pub" --priv-path "./data/id_rsa" -i ./plain-text -o ./data/sign-text
wait
../bin/rsa -m verify --pub-path "./data/id_rsa.pub" -i ./data/sign-text -o ./data/verify-text
wait