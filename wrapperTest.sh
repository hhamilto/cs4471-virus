#! /bin/bash
make
if [ $? != 0 ]
	then
	exit 1;
fi
#rm -rf /tmp/.*.39451
echo "AABAAAAAA" > foo;

cp virus seed
printf '\xde\xad\xbe\xef' >> seed
cat ls >> seed

chmod u+x seed

./seed foo

./tstWrappers foo