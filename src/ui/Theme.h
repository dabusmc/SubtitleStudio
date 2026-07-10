#pragma once

#include <QColor>
#include <QString>

namespace SubtitleStudio
{
    namespace Icons
    {
        inline constexpr auto Play = ":/icons/play.svg";
        inline constexpr auto Pause = ":/icons/pause.svg";

        inline constexpr auto Rewind = ":/icons/rewind.svg";
        inline constexpr auto FastForward = ":/icons/fast_forward.svg";

        inline constexpr auto PreviousSubtitle = ":/icons/previous_subtitle.svg";
        inline constexpr auto NextSubtitle = ":/icons/next_subtitle.svg";
    }

    namespace Theme
    {
        namespace Colours
        {
            inline const QColor Background(40, 40, 40);
            inline const QColor Panel(55, 55, 55);
            inline const QColor Surface(45, 45, 45);

            inline const QColor Text(240, 240, 240);
            inline const QColor SecondaryText(180, 180, 180);

            inline const QColor Border(80, 80, 80);

            inline const QColor Accent(70, 150, 255);
            inline const QColor AccentHover(90, 170, 255);
            inline const QColor AccentSelected(255, 180, 60);

            inline const QColor Track(55, 55, 55);
            inline const QColor Playhead(Qt::red);
        }

        namespace Metrics
        {
            constexpr int WindowMargin = 10;

            constexpr int CardPadding = 12;
            constexpr int CardSpacing = 12;

            constexpr int BorderRadius = 6;

            constexpr int TimelineMargin = 10;
            constexpr int TimelineTopPadding = 10;
            constexpr int TimelineTrackHeight = 30;
            constexpr int TimelineTrackSpacing = 10;
        }

        // Helpers
        inline QString ToCss(const QColor& colour)
        {
            return colour.name(QColor::HexRgb);
        }

        inline QString Border()
        {
            return QString("1px solid %1").arg(ToCss(Colours::Border));
        }
    }

    namespace Styles
    {
        inline QString ToolButton()
        {
            return QString(
                "QPushButton {"
                "background-color:%1;"
                "color:%2;"
                "border:1px solid %3;"
                "border-radius:%4px;"
                "padding:2px;"
                "}"
                "QPushButton:hover {"
                "background-color:%5;"
                "}"
                "QPushButton:pressed {"
                "background-color:%6;"
                "}")
                .arg(Theme::ToCss(Theme::Colours::Surface))
                .arg(Theme::ToCss(Theme::Colours::Text))
                .arg(Theme::ToCss(Theme::Colours::Border))
                .arg(Theme::Metrics::BorderRadius)
                .arg(Theme::ToCss(Theme::Colours::AccentHover))
                .arg(Theme::ToCss(Theme::Colours::Accent));
        }

        inline QString TimeLabel()
        {
            return QString(
                "QLabel {"
                "color:%1;"
                "font-family:Consolas;"
                "font-size:10pt;"
                "}")
                .arg(Theme::ToCss(Theme::Colours::Text));
        }

        inline QString Card()
        {
            return QString(
                "background-color:%1;"
                "border-radius:%2px;")
                .arg(Theme::ToCss(Theme::Colours::Panel))
                .arg(Theme::Metrics::BorderRadius);
        }

        inline QString ReadOnlyField()
        {
            return QString(
                "QLineEdit {"
                "background-color:%1;"
                "color:%2;"
                "border:1px solid %3;"
                "border-radius:%4px;"
                "padding:4px;"
                "}")
                .arg(Theme::ToCss(Theme::Colours::Surface))
                .arg(Theme::ToCss(Theme::Colours::Text))
                .arg(Theme::ToCss(Theme::Colours::Border))
                .arg(Theme::Metrics::BorderRadius);
        }

        inline QString TextEditor()
        {
            return QString(
                "QPlainTextEdit {"
                "background-color:%1;"
                "color:%2;"
                "border:1px solid %3;"
                "border-radius:%4px;"
                "padding:6px;"
                "selection-background-color:%5;"
                "}")
                .arg(Theme::ToCss(Theme::Colours::Surface))
                .arg(Theme::ToCss(Theme::Colours::Text))
                .arg(Theme::ToCss(Theme::Colours::Border))
                .arg(Theme::Metrics::BorderRadius)
                .arg(Theme::ToCss(Theme::Colours::Accent));
        }

        inline QString EditableField()
        {
            return QString(
                "QLineEdit {"
                "background-color:%1;"
                "color:%2;"
                "border:1px solid %3;"
                "border-radius:%4px;"
                "padding:4px;"
                "}"
                "QLineEdit:focus {"
                "border:2px solid %5;"
                "}")
                .arg(Theme::ToCss(Theme::Colours::Surface))
                .arg(Theme::ToCss(Theme::Colours::Text))
                .arg(Theme::ToCss(Theme::Colours::Border))
                .arg(Theme::Metrics::BorderRadius)
                .arg(Theme::ToCss(Theme::Colours::Accent));
        }

        inline QString Label()
        {
            return QString(
                "QLabel {"
                "color:%1;"
                "}")
                .arg(Theme::ToCss(Theme::Colours::SecondaryText));
        }
    }
}