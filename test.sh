SUM=0
for ((i=1;i<=$1;i++)); do
	ARG=`./rubik -s $2`
	echo $ARG
	SOL=`./rubik "$ARG"`
	echo $SOL
	REZ=`echo "$SOL" | wc -w`
	echo $REZ
#	./rubik "$ARG $SOL" | cat -e
	SUM=$(($SUM+$REZ))
done
SUM=$(($SUM/$1))
echo $SUM