.PHONY: help list new clean-all

help:
	@echo "System programming macOS/VS Code template"
	@echo ""
	@echo "Create an exercise:"
	@echo "  ./tools/new-exercise.sh single-c lab04/ex01"
	@echo "  ./tools/new-exercise.sh single-cpp lab04/ex02"
	@echo "  ./tools/new-exercise.sh pthread-c lab04/ex03"
	@echo "  ./tools/new-exercise.sh multifile-cpp lab04/ex04"
	@echo "  ./tools/new-exercise.sh async-cpp lab05/ex01"
	@echo ""
	@echo "Then:"
	@echo "  cd workspace/lab04/ex01 && make run"

list:
	@find workspace -name Makefile -maxdepth 5 -print | sed 's#/Makefile##'

clean-all:
	@find workspace -name Makefile -maxdepth 5 -execdir make clean \;
