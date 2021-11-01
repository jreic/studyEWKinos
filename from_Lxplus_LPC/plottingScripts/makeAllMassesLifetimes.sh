declare -a StringArray=("120_110_100" "200_190_180" )
for massString in ${StringArray[@]}; do
	for lifetime in 1 10 100 1000; do
		root -l -b -q "GentoRecoMatchingv2.C(\"$massString\", $lifetime)"
	done
done
