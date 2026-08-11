/* Copyright (c) 2020-2026 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xdisasmviewoptionswidget.h"

#include "ui_xdisasmviewoptionswidget.h"

XDisasmViewOptionsWidget::XDisasmViewOptionsWidget(QWidget *pParent) : XShortcutsWidget(pParent), ui(new Ui::XDisasmViewOptionsWidget)
{
    ui->setupUi(this);

    m_pOptions = nullptr;
    m_mode = MODE_ALL;

    setProperty("GROUPID", XOptions::GROUPID_DISASM);
}

XDisasmViewOptionsWidget::~XDisasmViewOptionsWidget()
{
    delete ui;
}

void XDisasmViewOptionsWidget::adjustView()
{
    // TODO
}

void XDisasmViewOptionsWidget::setOptions(XOptions *pOptions, MODE mode)
{
    m_pOptions = pOptions;
    m_mode = mode;

    reload();
}

void XDisasmViewOptionsWidget::save()
{
    if ((m_mode == MODE_ALL) || (m_mode == MODE_X86)) {  // Combo box is only populated for these modes
        m_pOptions->getComboBox(ui->comboBoxDisasmSyntax, XOptions::ID_DISASM_SYNTAX);
    }
    m_pOptions->getCheckBox(ui->checkBoxDisasmLocationColon, XOptions::ID_DISASM_LOCATIONCOLON);
    m_pOptions->getCheckBox(ui->checkBoxDisasmUppercase, XOptions::ID_DISASM_UPPERCASE);
    m_pOptions->getCheckBox(ui->groupBoxDisasmHighlight, XOptions::ID_DISASM_HIGHLIGHT);
}

void XDisasmViewOptionsWidget::setDefaultValues(XOptions *pOptions, MODE mode)
{
    pOptions->addID(XOptions::ID_DISASM_FONT, XOptions::getMonoFont().toString());

    if ((mode == MODE_ALL) || (mode == MODE_X86)) {
        pOptions->addID(XOptions::ID_DISASM_SYNTAX, "");
    }

    pOptions->addID(XOptions::ID_DISASM_LOCATIONCOLON, true);
    pOptions->addID(XOptions::ID_DISASM_HIGHLIGHT, true);
    pOptions->addID(XOptions::ID_DISASM_UPPERCASE, false);

    // --- Default syntax-highlight palette (light background) ---------------------------------------
    // Distinct, muted hues so mnemonic / number / register-class / control-flow tokens are easy to tell
    // apart. The previous defaults painted every register class AND call/ret/syscall the same harsh red
    // (so "call rax" was all red), and gave generic mnemonics the same blue as numbers. Each value is a
    // "main|background" pair; an empty background slot means transparent.
    const QString sMnemonic = QColor(0, 0, 160).name() + "|";     // generic opcode    - blue
    const QString sNumber = QColor(0, 120, 0).name() + "|";       // immediates        - green
    const QString sRef = QColor(0, 110, 140).name() + "|";        // memory / refs     - teal
    const QString sRegGen = QColor(150, 0, 0).name() + "|";       // general registers - dark red
    const QString sRegStack = QColor(170, 85, 0).name() + "|";    // stack pointer     - brown
    const QString sRegSeg = QColor(0, 128, 128).name() + "|";     // segment registers - teal
    const QString sRegDebug = QColor(128, 128, 0).name() + "|";   // debug registers   - olive
    const QString sRegIP = QColor(199, 21, 133).name() + "|";     // instruction ptr   - violet-red
    const QString sRegFlags = QColor(128, 0, 128).name() + "|";   // flags register    - purple
    const QString sRegFPU = QColor(85, 107, 47).name() + "|";     // fpu st()          - dark olive
    const QString sRegVec = QColor(46, 139, 87).name() + "|";     // xmm/ymm/zmm        - sea green
    const QString sOpCall = QColor(140, 0, 176).name() + "|";     // call / syscall    - purple
    const QString sOpJmp = QColor(0, 0, 205).name() + "|";        // unconditional jmp - blue
    const QString sOpCondJmp = QColor(30, 140, 30).name() + "|";  // conditional jmp   - green
    const QString sOpRet = QColor(178, 34, 34).name() + "|";      // ret               - firebrick
    const QString sOpStack = QColor(40, 100, 180).name() + "|";   // push / pop        - steel blue
    const QString sOpNop = QColor(150, 150, 150).name() + "|";    // nop               - gray
    const QString sOpInt3 = QColor(120, 120, 120).name() + "|";   // int3 / filler     - dark gray

    // Colors
    pOptions->addID(XOptions::ID_DISASM_COLOR_ARROWS, QString("%1|%2").arg("").arg(""));
    pOptions->addID(XOptions::ID_DISASM_COLOR_ARROWS_SELECTED, QString("%1|%2").arg(QColor(Qt::red).name()).arg(""));
    pOptions->addID(XOptions::ID_DISASM_COLOR_REGS, sRegGen);
    pOptions->addID(XOptions::ID_DISASM_COLOR_NUMBERS, sNumber);
    pOptions->addID(XOptions::ID_DISASM_COLOR_OPCODE, sMnemonic);
    pOptions->addID(XOptions::ID_DISASM_COLOR_REFS, sRef);
    // Current instruction pointer line: highlight lives in the background slot (translucent green by default).
    pOptions->addID(XOptions::ID_DISASM_COLOR_CURRENTIP, QString("%1|%2").arg("").arg(QColor(0, 160, 0, 90).name(QColor::HexArgb)));
    // X86
    if ((mode == MODE_ALL) || (mode == MODE_X86)) {
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_STACK, sRegStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_SEGMENT, sRegSeg);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_DEBUG, sRegDebug);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_IP, sRegIP);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_FLAGS, sRegFlags);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_FPU, sRegFPU);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_XMM, sRegVec);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_YMM, sRegVec);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_REGS_ZMM, sRegVec);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_NOP, sOpNop);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_INT3, sOpInt3);
        pOptions->addID(XOptions::ID_DISASM_COLOR_X86_OPCODE_SYSCALL, sOpCall);
    }

    if ((mode == MODE_ALL) || (mode == MODE_ARM)) {
        // ARM
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_REGS_STACK, sRegStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_OPCODE_BRANCH, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_OPCODE_BRANCHLINK, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_ARM_OPCODE_NOP, sOpNop);
        // TODO more
    }

    // Other architectures (only registered in the generic multi-arch mode)
    if (mode == MODE_ALL) {
        // MIPS
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MIPS_OPCODE_NOP, sOpNop);
        // PowerPC
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_PPC_OPCODE_NOP, sOpNop);
        // SPARC
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_SPARC_OPCODE_NOP, sOpNop);
        // M68K
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_M68K_OPCODE_NOP, sOpNop);
        // MOS 65xx
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_NOP, sOpNop);
        // BPF
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_REGS_GENERAL, sRegGen);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_CALL, sOpCall);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_RET, sOpRet);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_JMP, sOpJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_COND_JMP, sOpCondJmp);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_PUSH, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_POP, sOpStack);
        pOptions->addID(XOptions::ID_DISASM_COLOR_BPF_OPCODE_NOP, sOpNop);
    }
}

QList<DialogViewColors::RECORD> XDisasmViewOptionsWidget::getRecords(MODE mode)
{
    QList<DialogViewColors::RECORD> listResult;

    {
        QString sGroup = tr("Arrows");

        {
            DialogViewColors::RECORD record = {sGroup, tr("All"), XOptions::ID_DISASM_COLOR_ARROWS};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Selected"), XOptions::ID_DISASM_COLOR_ARROWS_SELECTED};
            listResult.append(record);
        }
    }
    {
        DialogViewColors::RECORD record = {"", tr("Registers"), XOptions::ID_DISASM_COLOR_REGS};
        listResult.append(record);
    }
    {
        DialogViewColors::RECORD record = {"", tr("Numbers"), XOptions::ID_DISASM_COLOR_NUMBERS};
        listResult.append(record);
    }
    {
        DialogViewColors::RECORD record = {"", tr("Opcodes"), XOptions::ID_DISASM_COLOR_OPCODE};
        listResult.append(record);
    }
    {
        DialogViewColors::RECORD record = {"", tr("References"), XOptions::ID_DISASM_COLOR_REFS};
        listResult.append(record);
    }
    {
        DialogViewColors::RECORD record = {tr("Debugger"), tr("Current instruction pointer"), XOptions::ID_DISASM_COLOR_CURRENTIP};
        listResult.append(record);
    }

    // TODO another assemblers
    if ((mode == MODE_ALL) || (mode == MODE_X86)) {
        QString sGroup = "x86/amd64";
        {
            DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_X86_REGS_GENERAL};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Stack registers"), XOptions::ID_DISASM_COLOR_X86_REGS_STACK};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Segment registers"), XOptions::ID_DISASM_COLOR_X86_REGS_SEGMENT};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Debug registers"), XOptions::ID_DISASM_COLOR_X86_REGS_DEBUG};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Instruction pointer register"), XOptions::ID_DISASM_COLOR_X86_REGS_IP};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Flags register"), XOptions::ID_DISASM_COLOR_X86_REGS_FLAGS};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, QString("FPU %1").arg(tr("Registers")), XOptions::ID_DISASM_COLOR_X86_REGS_FPU};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, QString("XMM %1").arg(tr("Registers")), XOptions::ID_DISASM_COLOR_X86_REGS_XMM};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, QString("YMM %1").arg(tr("Registers")), XOptions::ID_DISASM_COLOR_X86_REGS_YMM};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, QString("ZMM %1").arg(tr("Registers")), XOptions::ID_DISASM_COLOR_X86_REGS_ZMM};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_X86_OPCODE_CALL};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_X86_OPCODE_RET};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_X86_OPCODE_COND_JMP};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_X86_OPCODE_PUSH};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_X86_OPCODE_POP};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_X86_OPCODE_NOP};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_X86_OPCODE_JMP};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "INT3", XOptions::ID_DISASM_COLOR_X86_OPCODE_INT3};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "SYSCALL", XOptions::ID_DISASM_COLOR_X86_OPCODE_SYSCALL};
            listResult.append(record);
        }
    }

    if ((mode == MODE_ALL) || (mode == MODE_ARM)) {
        QString sGroup = "arm/arm64";
        {
            DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_ARM_REGS_GENERAL};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, tr("Stack registers"), XOptions::ID_DISASM_COLOR_ARM_REGS_STACK};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "Branch (B)", XOptions::ID_DISASM_COLOR_ARM_OPCODE_BRANCH};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "Branch with Link (BL)", XOptions::ID_DISASM_COLOR_ARM_OPCODE_BRANCHLINK};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_ARM_OPCODE_RET};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_ARM_OPCODE_PUSH};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_ARM_OPCODE_POP};
            listResult.append(record);
        }
        {
            DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_ARM_OPCODE_NOP};
            listResult.append(record);
        }
    }

    if (mode == MODE_ALL) {
        {
            QString sGroup = "MIPS";
            {
                DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_MIPS_REGS_GENERAL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_CALL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_RET};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_COND_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_PUSH};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_POP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_MIPS_OPCODE_NOP};
                listResult.append(record);
            }
        }
        {
            QString sGroup = "PowerPC";
            {
                DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_PPC_REGS_GENERAL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_PPC_OPCODE_CALL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_PPC_OPCODE_RET};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_PPC_OPCODE_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_PPC_OPCODE_COND_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_PPC_OPCODE_PUSH};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_PPC_OPCODE_POP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_PPC_OPCODE_NOP};
                listResult.append(record);
            }
        }
        {
            QString sGroup = "SPARC";
            {
                DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_SPARC_REGS_GENERAL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_CALL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_RET};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_COND_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_PUSH};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_POP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_SPARC_OPCODE_NOP};
                listResult.append(record);
            }
        }
        {
            QString sGroup = "M68K";
            {
                DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_M68K_REGS_GENERAL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_M68K_OPCODE_CALL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_M68K_OPCODE_RET};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_M68K_OPCODE_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_M68K_OPCODE_COND_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_M68K_OPCODE_PUSH};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_M68K_OPCODE_POP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_M68K_OPCODE_NOP};
                listResult.append(record);
            }
        }
        {
            QString sGroup = "MOS 65xx";
            {
                DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_MOS65XX_REGS_GENERAL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_CALL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_RET};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_COND_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_PUSH};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_POP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_MOS65XX_OPCODE_NOP};
                listResult.append(record);
            }
        }
        {
            QString sGroup = "BPF";
            {
                DialogViewColors::RECORD record = {sGroup, tr("General registers"), XOptions::ID_DISASM_COLOR_BPF_REGS_GENERAL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "CALL", XOptions::ID_DISASM_COLOR_BPF_OPCODE_CALL};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "RET", XOptions::ID_DISASM_COLOR_BPF_OPCODE_RET};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "JMP", XOptions::ID_DISASM_COLOR_BPF_OPCODE_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "COND JMP", XOptions::ID_DISASM_COLOR_BPF_OPCODE_COND_JMP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "PUSH", XOptions::ID_DISASM_COLOR_BPF_OPCODE_PUSH};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "POP", XOptions::ID_DISASM_COLOR_BPF_OPCODE_POP};
                listResult.append(record);
            }
            {
                DialogViewColors::RECORD record = {sGroup, "NOP", XOptions::ID_DISASM_COLOR_BPF_OPCODE_NOP};
                listResult.append(record);
            }
        }
    }
    return listResult;
}

void XDisasmViewOptionsWidget::reloadData(bool bSaveSelection)
{
    Q_UNUSED(bSaveSelection)
    reload();
}

void XDisasmViewOptionsWidget::reload()
{
    m_pOptions->setCheckBox(ui->checkBoxDisasmLocationColon, XOptions::ID_DISASM_LOCATIONCOLON);
    m_pOptions->setCheckBox(ui->groupBoxDisasmHighlight, XOptions::ID_DISASM_HIGHLIGHT);
    m_pOptions->setCheckBox(ui->checkBoxDisasmUppercase, XOptions::ID_DISASM_UPPERCASE);

    if ((m_mode == MODE_ALL) || (m_mode == MODE_X86)) {
        m_pOptions->setComboBox(ui->comboBoxDisasmSyntax, XOptions::ID_DISASM_SYNTAX);
    } else {
        ui->groupBoxDisasmSyntax->hide();
    }
}

void XDisasmViewOptionsWidget::on_pushButtonDisasmColors_clicked()
{
    DialogViewColors dialogColors(this);
    dialogColors.setGlobal(getShortcuts(), getGlobalOptions());

    QList<DialogViewColors::RECORD> listRecords = getRecords(m_mode);

    dialogColors.setOptions(m_pOptions, listRecords, tr("Colors"));

    dialogColors.exec();
}

void XDisasmViewOptionsWidget::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
