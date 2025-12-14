#!/home/brownie/Desktop/Universidad/2s_SO/Practicas/Parte\ 2.\ Llamadas\ al\ sistema\ POSIX/minishell/minishell

rm -r scripttest

mkdir scripttest
cd scripttest

cat > lscat.txt
echo hola > lscat.txt

# ESTO NO FUNCIONA
ls .. > lsn.txt 2> lserr.txt
ls ../.. > lsn.txt 2> lserr.txt
ls quiensoy >> lsapp.txt 2> lserr.txt

cat lserr.txt # Poder ver el error.

ls nuevoerrraro  >> lsapp.txt 2> lserr.txt
ls . >> lsapp.txt
ls .. >>  lsapp.txt

sort lsn.txt | cat > lssort.txt
