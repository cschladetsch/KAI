
for name in *.cpp
do
	mv $name ${name/Rho/Hlsl}
done
