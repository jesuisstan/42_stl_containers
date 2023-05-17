make fclean
make
echo "\n\033[33;1mACHTUNG!\n\033[0mtesting succeed"
./ft_containers > ft.txt
./stl_containers > stl.txt
diff ft.txt stl.txt > diff.txt
echo "\033[35;1mRESULTS\n\033[0mdifferences in output:\n"
cat diff.txt