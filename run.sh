echo "** creat ssd space :"
sudo rm -rf ~/ssd
mkdir ~/ssd
sudo mkfs -t ext4 /dev/nvme0n1 > ~/log/testlog
sudo mount /dev/nvme0n1 ~/ssd
echo " PASS \n"
echo "** git pull :"
git pull origin master
echo " PASS \n"
echo "** gcc  :"
gcc test.c -o test
echo " PASS \n"
sudo cp ~/test/test ~/ssd/test