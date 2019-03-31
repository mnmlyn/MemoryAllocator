obj=memalloc.o test_memalloc.o

prog:$(obj)
	g++ -g -o $@ $^

%.o:%.cpp
	g++ -g -c $<

.PHONY:clean
clean:
	rm -f $(obj) prog
