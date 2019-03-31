obj=memalloc.o test_memalloc.o

prog:$(obj)
	g++ -o $@ $^

%.o:%.cpp
	g++ -c $<

.PHONY:clean
clean:
	rm -f $(obj) prog
