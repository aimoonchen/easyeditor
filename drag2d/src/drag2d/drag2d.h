#pragma once

#include "interfaces.h"

#include "algorithms/DelaunayTriangulation.h"
#include "algorithms/Triangulation.h"
#include "algorithms/DouglasPeucker.h"
#include "algorithms/CosineSmooth.h"
#include "algorithms/MinBoundingBox.h"
#include "algorithms/ConvexHull.h"

#include "dataset/Bitmap.h"
#include "dataset/Image.h"
#include "dataset/RawPixels.h"
#include "dataset/ISymbol.h"
#include "dataset/NullSymbol.h"
#include "dataset/SymbolFactory.h"
#include "dataset/SymbolMgr.h"
#include "dataset/SymbolSearcher.h"
#include "dataset/SearcherPathMgr.h"
#include "dataset/ImageSymbol.h"
#include "dataset/FontBlankSymbol.h"
#include "dataset/ScriptsSymbol.h"
#include "dataset/ISprite.h"
#include "dataset/NullSprite.h"
#include "dataset/SpriteFactory.h"
#include "dataset/ImageSprite.h"
#include "dataset/TextSprite.h"
#include "dataset/FontSprite.h"
#include "dataset/BVFactory.h"
#include "dataset/ImageLoader.h"
#include "dataset/ImageSaver.h"
#include "dataset/Snapshoot.h"
#include "dataset/Font.h"
#include "dataset/AnimatedGifSaver.h"
#include "dataset/Layer.h"
#include "dataset/SpriteTools.h"
#include "dataset/AbstractBV.h"
#include "dataset/TPNode.h"
#include "dataset/text_util.h"

#include "view/OrthoCanvas.h"
#include "view/PerspectCanvas.h"
#include "view/Camera.h"
#include "view/LibraryPanel.h"
#include "view/ILibraryPage.h"
#include "view/LibraryList.h"
#include "view/LibraryImagePage.h"
#include "view/LibraryFontBlankPage.h"
#include "view/LibraryScriptsPage.h"
#include "view/PropertySettingPanel.h"
#include "view/IPropertySetting.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/TextPropertySetting.h"
#include "view/SymbolPropertySetting.h"
#include "view/DialogStageCanvas.h"
#include "view/ShapesPanelImpl.h"
#include "view/SpritesPanelImpl.h"
#include "view/MultiShapesImpl.h"
#include "view/MultiSpritesImpl.h"
#include "view/ToolbarPanel.h"
#include "view/ViewlistPanel.h"
#include "view/ExceptionDlg.h"
#include "view/ExitDlg.h"
#include "view/Frame.h"
#include "view/VectorUtils.h"
#include "view/SettingsDialog.h"
#include "view/FinishDialog.h"
#include "view/LibraryFontPage.h"
#include "view/TextDialog.h"
#include "view/StageDropTarget.h"
#include "view/CombinedDropData.h"
#include "view/IDataContainer.h"
#include "view/FontPropertySetting.h"

#include "operator/AbstractEditOP.h"
#include "operator/NullEditOP.h"
#include "operator/ZoomViewOP.h"
#include "operator/ArrangeSpriteOP.h"
#include "operator/ArrangeSpriteFixOP.h"
#include "operator/SelectSpritesOP.h"
#include "operator/DrawRectangleOP.h"
#include "operator/PasteSymbolOP.h"
#include "operator/PasteSymbolRandomOP.h"
#include "operator/PasteSymbolCaptureOP.h"
#include "operator/PasteSymbolTileOP.h"
#include "operator/PasteSpriteOP.h"
#include "operator/SelectShapesOP.h"
#include "operator/TranslateSpriteState.h"
#include "operator/RotateSpriteState.h"
#include "operator/ScaleSpriteState.h"
#include "operator/MousePositionOP.h"

#include "history/AbstractAtomicOP.h"

#include "component/AbstractEditCMPT.h"
#include "component/UniversalCMPT.h"
#include "component/PasteSymbolScaleCMPT.h"
#include "component/PasteSymbolRandomCMPT.h"
#include "component/PasteSymbolOffsetCMPT.h"
#include "component/PasteSpriteCMPT.h"
#include "component/MousePositionCMPT.h"

#include "widgets/SetValueDialog.h"
#include "widgets/ListItem.h"
#include "widgets/VerticalImageList.h"
#include "widgets/DragTargetNull.h"
#include "widgets/SelectDrawTypeWidget.h"
#include "widgets/LayersMgrWidget.h"
#include "widgets/PasteSymbolRandomWidget.h"
#include "widgets/SliderCtrl.h"
#include "widgets/HSLColorSettingDlg.h"
#include "widgets/RGBColorSettingDlg.h"
#include "widgets/ColorProperty.h"

#include "common/LibJpeg.h"
#include "common/log.h"
#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/TexPackerAdapter.h"
#include "common/Settings.h"
#include "common/Math.h"
#include "common/math_common.h"
#include "common/visitors.h"
#include "common/tools.h"
#include "common/Random.h"
#include "common/Exception.h"
#include "common/color_trans.h"
#include "common/color_config.h"
#include "common/runtime.h"
#include "common/scripts.h"
#include "common/JsonTools.h"
#include "common/PointerPool.h"
#include "common/filetools.h"
#include "common/Statistics.h"
#include "common/PlayControl.h"
#include "common/FpsStat.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "common/const.h"
#include "common/color_convert.h"

#include "render/PrimitiveDraw.h"
#include "render/DrawSelectedShapeVisitor.h"
#include "render/DrawShapesVisitor.h"
#include "render/DrawSelectedSpriteVisitor.h"
#include "render/DrawSpritesVisitor.h"
#include "render/SpriteDraw.h"
#include "render/SpriteBatch.h"
#include "render/style_config.h"
#include "render/ShaderMgr.h"
//#include "render/DynamicTexture.h"
//#include "render/DynamicFont.h"
#include "render/RenderContext.h"
#include "render/IShader.h"
#include "render/LabelNew.h"
#include "render/SpriteShader.h"