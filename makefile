# 刪除檔案指令
rm        = rm -f

# 各式資料夾名稱定義
# 原始碼資料夾
SRCDIR    = src
# 標頭檔資料夾
INCDIR    = inc
# 編譯目標檔資料夾
OBJDIR    = obj
# 執行檔資料夾
BINDIR    = bin
# 單元測試資料夾
TESTDIR = test
# 單元測試結果資料夾
RESULTDIR = result


# 副檔名定義
# 原始碼副檔名
SRCEXT	  = .cpp
# 標頭檔副檔名
HEADEREXT = .hpp
# 執行黨副檔名
EXEEXT = 

# 編譯指令
COMPILE = clang++ -g -Wall -O2 -std=c++17
# 編譯時 include 路徑 (--cflags)
COMPILE_INCS = -I ./inc
# 單元測試編譯時 include 路徑 (--cflags)
TEST_COMPILE_INCS = $(COMPILE_INCS) `pkg-config gtest --cflags`

# 連結指令
LINK = clang++
# 連結時函示庫路徑 (--libs)
LINK_LIBS =
# 單元測試連結時函示庫路徑 (--libs)
TEST_LINK_LIBS = `pkg-config gtest --libs`

# 檔案掃描
# 進入點檔案要在此定義
# 輸出執行檔檔名
ENTRY        = src/lrc-parser$(SRCEXT) src/calc-parser$(SRCEXT)
ENRTYOBJS    := $(ENTRY:$(SRCDIR)/%$(SRCEXT)=$(OBJDIR)/%.o)
ENRTYEXES    := $(ENTRY:$(SRCDIR)/%$(SRCEXT)=$(BINDIR)/%$(EXEEXT))
ALLSRCS 	:= $(shell find $(SRCDIR) -type f -name \*$(SRCEXT))
ALLHEADERS  := $(shell find $(INCDIR) -type f -name \*$(HEADEREXT))
SRCS        := $(filter-out $(ENTRY), $(ALLSRCS))
OBJS        := $(SRCS:$(SRCDIR)/%$(SRCEXT)=$(OBJDIR)/%.o)
INCS        := $(ALLHEADERS)

ALLTSRCS    := $(shell find $(TESTDIR)/$(SRCDIR) -type f -name \*$(SRCEXT))
TSRCS       := $(ALLTSRCS)
TOBJS       := $(TSRCS:$(TESTDIR)/$(SRCDIR)/%$(SRCEXT)=$(TESTDIR)/$(OBJDIR)/%.o)
TTARGETS    := $(TSRCS:$(TESTDIR)/$(SRCDIR)/%$(SRCEXT)=$(TESTDIR)/$(BINDIR)/%)
TRESULTS    := $(TSRCS:$(TESTDIR)/$(SRCDIR)/%$(SRCEXT)=$(TESTDIR)/$(RESULTDIR)/%.txt)

# 顏色代碼
# 標頭顏色
HC          = \033[1;33m
# 路徑顏色
PC          = \033[1;34m
# 無顏色
NC          = \033[0m

# 編譯輸出執行檔
all: init $(ENRTYEXES)
	@echo Make All Complete


# 編譯輸出執行檔之初始化
init: 
	@echo "$(HC)Target   :$(NC) $(PC)$(ENRTYEXES)$(NC)"
	@echo "$(HC)Entry    :$(NC) $(PC)$(ENTRY)$(NC)"
	@echo "$(HC)Entry Obj:$(NC) $(PC)$(ENRTYOBJS)$(NC)"
	@echo "$(HC)Srcs     :$(NC) $(PC)$(SRCS:%=\n\t%)$(NC)"
	@echo "$(HC)Objs     :$(NC) $(PC)$(OBJS:%=\n\t%)$(NC)"
	@echo "$(HC)Incs     :$(NC) $(PC)$(INCS:%=\n\t%)$(NC)"
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)
	@cd $(SRCDIR) && find . -type d -exec mkdir -p -- ../$(OBJDIR)/{} \;

# 單元測試編譯執行存檔
test: tinit  $(TTARGETS) remove-test-result $(TRESULTS) 
	@echo Test Complete

# 單元測試編譯執行存檔之初始化
tinit: init
	@echo "$(HC)Test Srcs   :$(NC) $(PC)$(TSRCS)$(NC)"
	@echo "$(HC)Test Objs   :$(NC) $(PC)$(TOBJS)$(NC)"
	@echo "$(HC)Test Targets:$(NC) $(PC)$(TTARGETS)$(NC)"
	@echo "$(HC)Test Results:$(NC) $(PC)$(TRESULTS)$(NC)"
	@mkdir -p $(TESTDIR)
	@cd $(TESTDIR) && mkdir -p $(BINDIR)
	@cd $(TESTDIR) && mkdir -p $(OBJDIR)
	@cd $(TESTDIR) && mkdir -p $(RESULTDIR)

# 執行單元測試
$(TRESULTS): 
	@echo "Testing $(PC)$(@:test/result%.txt=test/bin%)$(NC)"
	@echo "Result will saved at $(PC)$@$(NC)"
	@$(@:$(TESTDIR)/$(RESULTDIR)%.txt=$(TESTDIR)/$(BINDIR)%) 2> /dev/null | tee $@

# 連結目標輸出執行檔
$(ENRTYEXES): $(ENRTYOBJS) $(OBJS)
	@$(LINK) $(@:$(BINDIR)/%=$(OBJDIR)/%.o) $(OBJS) $(LINK_LIBS) -o $@
	@echo "Linking complete!"

# 連結單元測試目標輸出單元測試執行檔
$(TTARGETS): $(TOBJS) $(OBJS)
	@$(LINK) $(@:$(TESTDIR)/$(BINDIR)%=$(TESTDIR)/$(OBJDIR)%.o) $(OBJS) $(TEST_LINK_LIBS) -o $@
	@echo "Linking $(PC)$(@:$(TESTDIR)/$(BINDIR)%=$(TESTDIR)/$(OBJDIR)%.o)$(NC) to $(PC)$@$(NC) complete!"

# 編譯原始碼
$(ENRTYOBJS) $(OBJS) : $(OBJDIR)/%.o : $(SRCDIR)/%$(SRCEXT)
	@$(COMPILE) $(COMPILE_INCS) -c $< -o $@
	@echo "Compile from $(PC)"$<"$(NC) to $(PC)"$@"$(NC) successfully!"

# 編譯單元測試原始碼
$(TOBJS) : $(TESTDIR)/$(OBJDIR)/%.o : $(TESTDIR)/$(SRCDIR)/%$(SRCEXT)
	@$(COMPILE) $(TEST_COMPILE_INCS) -c $< -o $@
	@echo "Compile from $(PC)"$<"$(NC) to $(PC)"$@"$(NC) successfully!"


# 清除編譯目標檔並移除執行檔
.PHONY: remove
remove: clean
	@$(rm) $(TARGET)
	@echo "Executable removed!"

# 清除單元測試編譯目標並移除單元測試執行檔
.PHONY: remove-test
remove-test: clean clean-test
	@$(rm) $(TTARGETS)
	@echo "Test Executable removed!"

# 移除單元測試結果
.PHONY: remove-test-result
remove-test-result:
	@$(rm) $(TRESULTS)
	@echo "Result removed!"

# 清除編譯目標檔
.PHONY: clean
clean:
	@$(rm) $(OBJS) $(ENRTYOBJ)
	@echo "Cleanup complete!"


# 清除單元測試編譯目標
.PHONY: clean-test
clean-test:
	@$(rm) $(TOBJS)
	@echo "Test Cleanup complete!"

# 全部中間檔與執行檔刪除
.PHONY: remove-all
remove-all: remove remove-test remove-test-result
	@echo "Complete Removed All!"