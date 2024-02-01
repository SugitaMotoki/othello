.PHONY: clean
clean:
				make clean_v1
				make clean_v2

.PHONY: clean_v1
clean_v1:
				make -C v1/ allclean

.PHONY: clean_v2
clean_v2:
				make -C v2/ clean