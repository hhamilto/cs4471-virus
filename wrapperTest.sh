#! /bin/bash
make
if [ $? != 0 ]
	then
	exit 1;
fi
#rm -rf /tmp/.*.39451
rm -rf /tmp/.*.1000
echo "AABAAAAAA" > foo;

cp virus seed
printf '\xde\xad\xbe\xef' >> seed
cat ls >> seed

chmod u+x seed

./seed foo
echo "[ wrapperTest.sh ] ./tstWrappers :"
./tstWrappers foo