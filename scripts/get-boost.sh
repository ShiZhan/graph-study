export SCRIPT_DIR=$( cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd )
export INSTALL_DIR=$( pwd )
export BOOST_DIR="$INSTALL_DIR/boost_1_57_0"
wget http://jaist.dl.sourceforge.net/project/boost/boost/1.57.0/boost_1_57_0.tar.gz
echo "$( tar -xzvf boost_1_57_0.tar.gz | wc -l ) files extracted."
sed -in-place "s/BOOST_DIR = .*$/BOOST_DIR = ${BOOST_DIR}/g" $SCRIPT_DIR/../src/Makefile
