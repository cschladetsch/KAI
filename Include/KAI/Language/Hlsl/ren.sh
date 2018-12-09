
for name in *.h
do
	mv $name ${name/Rho/Hlsl}
done
