obj=memalloc.o test_memalloc.o

prog:$(obj)
	g++ -o $@ $^ -lgtest -lpthread

%.o:%.cpp
	g++ -g -c $<

.PHONY:clean
clean:
	rm -f $(obj) prog
