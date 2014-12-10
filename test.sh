#! /bin/bash
make
if [ $? != 0 ]
	then
	exit 1;
fi
chmod a-x ls

rm -f /tmp/host.39451 mls1 mls2
cp ls mls1
cp ls mls2

cp virus seed
printf '\xde\xad\xbe\xef' >> seed
cat ls >> seed

chmod u+x seed

./seed mls1

chmod u+x mls1

./mls1 mls2

ls -la
