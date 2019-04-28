#ifndef __UIOPTION_SCREEN_H__
#define __UIOPTION_SCREEN_H__

#pragma once

namespace DuiLib
{
    class UILIB_API COptionUIScreen : public COptionUI
    {
        DECLARE_DUICONTROL(COptionUIScreen)
    public:
        COptionUIScreen();
        ~COptionUIScreen();

        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);

        virtual void DoEvent(TEventUI& event);

    private:
        bool IsNormalOfDisplay();

    };
} // namespace DuiLib

#endif // __UIOPTION_SCREEN_H__