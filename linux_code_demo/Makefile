##################################
# @brief:make scripts
# @date:2017.03.16
# @author:john
##################################

CPP = g++
INC += -I/usr/include/mysql
LIBDIR += -L/usr/lib64/mysql

OBJECT = mysqlHelper.o socket_svr.o
TARGET = socket_svr

${TARGET}:${OBJECT}
	${CPP} $^ $(INC) $(LIBDIR) -lmysqlclient -o $@

${OBJECT}:%.o:%.cpp
	${CPP} -c $< -o $@ $(INC) $(LIBDIR) -lmysqlclient

.PHONY:clean
clean:
	rm -f ${TARGET} ${OBJECT}